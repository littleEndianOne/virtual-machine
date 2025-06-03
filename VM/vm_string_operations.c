#include "../VM/vm_string_operations.h"

#include <stdio.h>
#include "../VM/vm_opstack_operations.h"
#include "../VM/vm_progmem_operations.h"

/*
 * Read a string literal from program memory into memory allocation at strLit.
 * If the string is longer than strAllocSize then it is truncated.
 * The string is always terminated by a \0. 
 */
void ReadStringLit(vm_cpu* vm, vm_strlit strLit, char* dest, uint8_t strAllocSize) {
    uint8_t maxStrLength = strAllocSize - 1;
    uint8_t i;
    //Read characters into memory.
    for (i = 0; (i < strLit.length) & (i < maxStrLength); i++) //String length does not include the prefixed length.
    {
        dest[i] = (char) PeakCodeAt(vm, strLit.address + i);

        /*Performance tweak - Implement bulk read that checks the requested range of addresses is valid and then reads the bytes within that range.
        PeakCodeAt does a range check that can trigger segmentation faults. */
    }
    dest[i] = '\0'; //append string null terminator
}

/*
 * Copy string at src to allocation at dest.
 * If the string is longer than strAllocSize then it is truncated.
 * The string is always terminated by a \0. 
 */
void DuplicateString(char* src, char* dest, uint8_t allocLength) {
    uint8_t maxStrLength = allocLength - 1;
    uint8_t i = 0; //Start iteration at first character

    //Read characters into memory.
    while ((src[i] != '\0') & (i < maxStrLength)) {
        dest[i] = src[i];
        i++;
    }
    //append string null terminator
    dest[i] = '\0';
}

 /*
 * Converts "number" into a string with "precision".
 * If the number is to big for the available string size (strSize) then the 
 * output string will be truncated to the available memory.
 * 
 */
void FloatToString(float number, uint8_t precision, char* str, uint8_t strSize) {
    char formatStr[7]; //precision can be up to 255 - "%.255f\0" => 7 chars

    sprintf(formatStr, "%%.%if", precision); //construct format string.
    //printf("%s\n", formatStr);
    /*
    Could use snprintf_P when porting to AVR
    - Variant of snprintf() that uses a fmt string that resides in program memory.
     */
    snprintf(str, strSize, formatStr, number); // \0 is included in the string size count.      
}

/*
 * Converts integer into a string.
 * If the number is to big for the available string size (strSize) then the 
 * output string will be truncated to the available memory.
 */
void IntToString(int32_t number, char* str, uint8_t strSize) {    
    snprintf(str, strSize, "%i", number); // \0 is included in the string size count.      
}

/*
 * Pops a float or integer and a string reference. Converts the number into a 
 * string and stores it in the passed string reference.
 */
void NumToString(vm_cpu* vm) {
    uint8_t precision;   
    //Get number from operand stack
    vm_element number = OpStackPop(vm);

    //Get string to store converted number
    vm_element string = OpStackPop(vm);
    
    //Check for string type - prevent seg_fault.
    #ifdef DEV_ERROR_CHECKING   
    if (string.type != STRING_REF)
        SYSTEM_ERROR(TYPE_ERROR);    
    #endif // ERROR_CHECKING_ENABLED
    
    switch (number.type)
    {
        case FLOAT:
            precision = NextCode(vm);
            FloatToString(number.value.float32, precision, string.value.strRef.charPtr, string.value.strRef.allocLength);
            break;
        case INTEGER:
            IntToString(number.value.int32, string.value.strRef.charPtr, string.value.strRef.allocLength);
            break;
        default:
            SYSTEM_ERROR(TYPE_ERROR);
            break;            
    }   
}

/*
 Appends any vm_element type onto the end of an existing string.
 * Floats/Integers are converted into strings and appended.
 * Pops the element to be appended and the destination string from the stack.
 * If the string being appended is too large for the allocation it will be 
 * truncated.
 */
void AppendToStr(vm_cpu* vm) {
    //Get operand to be appended from operand stack
    vm_element source = OpStackPop(vm);

    //Get operand to be appended to from operand stack.
    vm_element dest = OpStackPop(vm);
    
    #ifdef DEV_ERROR_CHECKING
    if (dest.type != STRING_REF) {
        SYSTEM_ERROR(TYPE_ERROR);
    } 
    #endif // ERROR_CHECKING_ENABLED 

    //Find the end of the destination string and calculate the remaining bytes.
    char* remainingString = dest.value.strRef.charPtr;
    uint8_t charCount = 0;
    
    //Count the characters already in the string.
    while (*remainingString != '\0') {
        charCount++;
        remainingString++;
    }

    //Calculate remaining characters.
    uint8_t remainingAllocation;
    uint8_t precission;
    remainingAllocation = dest.value.strRef.allocLength - charCount;
    switch (source.type)
    {
        case FLOAT:
            precission = NextCode(vm);
            FloatToString(source.value.float32, precission, remainingString, remainingAllocation);
            break;
        case STRING_REF:
            DuplicateString(source.value.strRef.charPtr, remainingString, remainingAllocation);
            break;
        case STRING_LIT:
            ReadStringLit(vm, source.value.strLit, remainingString, remainingAllocation);
            break;
        case INTEGER:
            IntToString(source.value.int32, remainingString, remainingAllocation);
            break;
        default:
            SYSTEM_ERROR(TYPE_ERROR);    
            break;       
    }   
}

/* 
 *  
 */
void DeclareString(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize) {
    //Get length of the string allocation to create
    uint8_t size = NextCode(vm);

    #ifdef DEV_ERROR_CHECKING
    if (size == 0)
        SYSTEM_ERROR(INVALID_PARAM);
    #endif // ERROR_CHECKING_ENABLED 

    //Get address to store string
    uint8_t address = NextCode(vm);

    //Check if address is in range - prevent seg_fault.   
    #ifdef DEV_ERROR_CHECKING   
    if (!(address < varArraySize))
        SYSTEM_ERROR(MEM_SEG_FAULT); 
    #endif // ERROR_CHECKING_ENABLED 
    
    varArray[address].type = STRING_REF;
    varArray[address].value.strRef.allocLength = size;
    varArray[address].value.strRef.charPtr = (char*) malloc(sizeof (char)*size);

    //Initialise empty string  
    varArray[address].value.strRef.charPtr[0] = '\0';

    //Check that memory allocation worked   
   #ifdef DEV_ERROR_CHECKING   
    if (varArray[address].value.strRef.charPtr == NULL)
        SYSTEM_ERROR(MALLOC_FAULT);
    #endif // ERROR_CHECKING_ENABLED 
}



