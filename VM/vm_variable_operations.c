#include "../VM/vm_variable_operations.h"

#include <stdio.h>
#include "../VM/vm_common.h"
#include "../VM/vm_opstack_operations.h"
#include "../VM/vm_progmem_operations.h"
#include "../VM/vm_string_operations.h"

/*    
    Allocates memory for the passed array size and returns a pointer to the 
    array.
    Each array element has its type field set to the default value NONE.

    System Errors:
    MALLOC_FAULT - Memory could not be allocated.
 */
vm_element* CreateVariablesArray(uint8_t varCount) {
    
    vm_element* varArray = malloc(sizeof (vm_element) * varCount);   
    //If allocation failed then return NULL
    #ifdef DEV_ERROR_CHECKING
    if (varArray == NULL)
        return NULL;   
    #endif // DEV_ERROR_CHECKING

    uint8_t i;
    for (i = 0; i < varCount; i++) {
        varArray[i].type = NONE;
        //varArray[i].value.int32 = 0; //Clear any artifacts from memory.
        /*Removed 0 init for values as it will slow down function calls.
         * Programmer must ensure they don't use values of type NONE as numeric
         * operators don't do any type checking to improve performance.
         * 
         */
    }
    return varArray; 
}

/*
 * Frees all allocated memory used for local variable storage.
 * Frees any memory allocated to pointers within the array before freeing the 
 * memory the array itself resides in.
 */
void FreeVariablesArray(vm_element* varArray, uint8_t varCount) {
    uint8_t i;
    //Free memory allocated for storing strings and arrays.
    for (i = 0; i < varCount; i++) {
        if (varArray[i].type == STRING_REF) {
            free(varArray[i].value.strRef.charPtr);
        }
        else if (varArray[i].type == ARRAY_REF) {
            //Make recursive call to this function to free the array.
            FreeVariablesArray(varArray[i].value.arrayPtr->data, varArray[i].value.arrayPtr->size);
            free(varArray[i].value.arrayPtr);
        }
    }
    free(varArray); //local variable array allocation
}

/* Used to load locals and globals from a vm_element array.
 * Pops the address from the stack and calls the general Load() function
 * to load the element from the array.
 * 
 */
void LoadVariable(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize) {
    uint8_t address; //Address relative to the beginning of the current frames memory allocation.
    address = NextCode(vm); //little endian

    Load(vm, varArray, varArraySize, address);
}

/* Used to load vm_elements from local, global, and array vm_element arrays.
 * Checks that the address is in range and pushed the element onto the stack.
 */
void Load(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize, uint8_t address)
{
    //Is address valid (prevent seg_fault)
    #ifdef DEV_ERROR_CHECKING
    if (!(address < varArraySize))
        SYSTEM_ERROR(MEM_SEG_FAULT);
    #endif // DEV_ERROR_CHECKING
    
//    #ifdef DEV_ERROR_CHECKING
//    if (varArray[address].type == NONE)
//        SYSTEM_ERROR(TYPE_NOT_SET);
//    #endif // DEV_ERROR_CHECKING

    OpStackPush(vm, varArray[address]);
}

/*
 * Store passed vm_element 'float' in locals array pointed to by 'varArray' at index 'address'.
 * Assumes that the element passed is correct type and within bounds.
 * 
 * Type checking - 
 */
void StoreFloat(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element srcFloat) {
    #ifdef DEV_ERROR_CHECKING
    //if ((varArray[address].type == STRING_REF) | (varArray[address].type == ARRAY_REF))
    if ((varArray[address].type != NONE) & (varArray[address].type != FLOAT))
        SYSTEM_ERROR(TYPE_ERROR);
    #endif // ERROR_CHECKING_ENABLED
    varArray[address] = srcFloat;
}

/*
 * Store passed vm_element 'integer' in locals array pointed to by 'varArray' at index 'address'.
 * Assumes that the element passed is correct type and within bounds. 
 */
void StoreInt(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element srcInt) {
    #ifdef DEV_ERROR_CHECKING
    //if ((varArray[address].type == STRING_REF) | (varArray[address].type == ARRAY_REF))
    if ((varArray[address].type != NONE) & (varArray[address].type != INTEGER))
        SYSTEM_ERROR(TYPE_ERROR);
    #endif // ERROR_CHECKING_ENABLED
    varArray[address] = srcInt;
}

/*
 * Store passed vm_element 'strLit' in locals array pointed to by 'varArray' at index 'address'.
 * Assumes that the element passed is of type strLit.
 * 
 * The string is read from progmem and copied into allocated memory. 
 * The string is stored as a string reference (STRING_REF) element.
 */
void StoreStrLit(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element strLit) {
    #ifdef DEV_ERROR_CHECKING
    if ((varArray[address].type != NONE) & (varArray[address].type != STRING_REF))
        SYSTEM_ERROR(TYPE_ERROR);
    #endif // ERROR_CHECKING_ENABLED
      

    //If type == NOT_SET allocate memory to store the string.
    //Create a string reference at the passed address.
    if (varArray[address].type != STRING_REF) {
        varArray[address].value.strRef.charPtr = malloc(sizeof (char)*(strLit.value.strLit.length + 1)); //+1 for /0 term.

        //Check that memory allocation worked
        #ifdef DEV_ERROR_CHECKING
        if (varArray[address].value.strRef.charPtr == NULL)
            SYSTEM_ERROR(MALLOC_FAULT);
        #endif // ERROR_CHECKING_ENABLED

        varArray[address].value.strRef.allocLength = strLit.value.strLit.length + 1;
        varArray[address].type = STRING_REF; //Set the type
    }

    ReadStringLit(vm, strLit.value.strLit, varArray[address].value.strRef.charPtr, varArray[address].value.strRef.allocLength);

}



/* Copies the contents of 'srcStrRefE' into a variable within 'varArray'
 * at 'address'.
 * If the destination is already a string type the existing mem allocation will 
 * be used and the string truncated if it does not fit.
 * If the destination is not already a string type the memory allocation size
 * will be the same as the allocation size for the source string. 
 */
void StoreStrRef(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element srcStrRef) {
    #ifdef DEV_ERROR_CHECKING
    if ((varArray[address].type != NONE) & (varArray[address].type != STRING_REF))
        SYSTEM_ERROR(TYPE_ERROR);
    #endif // ERROR_CHECKING_ENABLED

    //If type == NOT_SET allocate memory to store the string.
    //Create a string reference at the passed address.
    if (varArray[address].type == NONE) {
        varArray[address].value.strRef.charPtr = malloc(sizeof (char)*srcStrRef.value.strRef.allocLength); //Same size as original allocation.

        //Check that memory allocation worked
        if (varArray[address].value.strRef.charPtr == NULL)
            SYSTEM_ERROR(MALLOC_FAULT);              

        varArray[address].value.strRef.allocLength = srcStrRef.value.strRef.allocLength;
        varArray[address].type = STRING_REF; //Set the type
    }
    DuplicateString(srcStrRef.value.strRef.charPtr, varArray[address].value.strRef.charPtr, varArray[address].value.strRef.allocLength);
}

/*
 * Copy the contents of srcArrayRefE into an array.
 * 
 */
void StoreArrayRef(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element srcArrayRef) {
    SYSTEM_ERROR(NOT_IMPLEMENTED);
    //Copy array to new location.
}

/*
 * Called to store local and global variables in the passed vm_element array.
 * Reads the address of the variable to store and then calls the Store function.
 * 
 */
void StoreVariable(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize) {
    uint8_t address = NextCode(vm);
    Store(vm, varArray, varArraySize, address);
}
 
/*
 * Stores a vm_element into the passed varArray at address.
 * Checks that address is less than varArraySize
 * Pops the element to be store from the stack.
 * Switches to the relevant storage function for the element type.
 * Used to store variables and array elements.
 */
void Store(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize, uint8_t address) {
    //Check for valid address (prevent seg_fault)    
    
    #ifdef DEV_ERROR_CHECKING   
    if (!(address < varArraySize))
        SYSTEM_ERROR(MEM_SEG_FAULT);    
    #endif // ERROR_CHECKING_ENABLED 
    
    //get value from top of the stack ...
    vm_element e = OpStackPop(vm);

    //branch based on the type.
    switch (e.type) {
        case FLOAT:
            StoreFloat(vm, varArray, address, e);
            break;
        case STRING_LIT:
            StoreStrLit(vm, varArray, address, e);
            break;
        case STRING_REF:
            StoreStrRef(vm, varArray, address, e);
            break;
        case ARRAY_REF:
            StoreArrayRef(vm, varArray, address, e);
            break;   
        case INTEGER:
            StoreInt(vm, varArray, address, e);
            break;
        default:
            SYSTEM_ERROR(TYPE_ERROR);
            break;            
    }
}
    

