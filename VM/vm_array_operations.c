#include "../VM/vm_array_operations.h"

#include <stdio.h>
#include "../VM/vm_common.h"
#include "../VM/vm_opstack_operations.h"
#include "../VM/vm_progmem_operations.h"
#include "../VM/vm_variable_operations.h"

/*
 * 
 */
void DeclareArray(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize) {
    //Get length of the array allocation to create
    uint8_t size = NextCode(vm);

    #ifdef DEV_ERROR_CHECKING
    if (size == 0)
        SYSTEM_ERROR(INVALID_PARAM);
    #endif // ERROR_CHECKING_ENABLED 

    //Get address to store string version of number into
    uint8_t address = NextCode(vm);

    //Check if address is in range    
    #ifdef DEV_ERROR_CHECKING
    if (!(address < varArraySize))
        SYSTEM_ERROR(MEM_SEG_FAULT);
    #endif // ERROR_CHECKING_ENABLED 
    
    varArray[address].type = ARRAY_REF;
    varArray[address].value.arrayPtr = (vm_array*) malloc(sizeof(vm_array));
    
    //Check that memory allocation worked    
    #ifdef DEV_ERROR_CHECKING
    if (varArray[address].value.arrayPtr == NULL)
        SYSTEM_ERROR(MALLOC_FAULT);
    #endif // ERROR_CHECKING_ENABLED 
    
    varArray[address].value.arrayPtr->size = size;      
    varArray[address].value.arrayPtr->data = CreateVariablesArray(size);     
    #ifdef DEV_ERROR_CHECKING
    if (varArray[address].value.arrayPtr->data == NULL)
        SYSTEM_ERROR(MALLOC_FAULT); 
    #endif // ERROR_CHECKING_ENABLED 
}

void StoreArrayElement(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize)
{
    uint8_t address = NextCode(vm);
    
    //Check if variable address is in range    
    #ifdef DEV_ERROR_CHECKING
    if (!(address < varArraySize))
        SYSTEM_ERROR(MEM_SEG_FAULT);   
    #endif // ERROR_CHECKING_ENABLED 
    
    //Pop index
    int32_t index = OpStackPopType(vm, INTEGER).int32;
    
    #ifdef DEV_ERROR_CHECKING
    //Check that variable at address is an array reference.    
    if (!(varArray[address].type == ARRAY_REF))
        SYSTEM_ERROR(TYPE_ERROR);    
    
    //Check that index is within bounds.    
    if (!(index < varArray[address].value.arrayPtr->size))
        SYSTEM_ERROR(MEM_SEG_FAULT);    
    #endif // ERROR_CHECKING_ENABLED 
    
    //Array is vm_element array like local and global variable arrays.
    Store(vm, varArray[address].value.arrayPtr->data, varArray[address].value.arrayPtr->size, index);
}

void LoadArrayElement(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize) {
    uint8_t address = NextCode(vm); //Get variable address.
    
    //Check if variable address is in range   
    #ifdef DEV_ERROR_CHECKING
    if (!(address < varArraySize))
        SYSTEM_ERROR(MEM_SEG_FAULT);    
    #endif // ERROR_CHECKING_ENABLED 
    //Pop index
    int32_t index = OpStackPopType(vm, INTEGER).int32;
    
    #ifdef DEV_ERROR_CHECKING
    if (!(varArray[address].type == ARRAY_REF))
        SYSTEM_ERROR(TYPE_ERROR);    
    #endif // ERROR_CHECKING_ENABLED 
    
    //Array is vm_element array like local and global variable arrays.
    Load(vm, varArray[address].value.arrayPtr->data, varArray[address].value.arrayPtr->size, index);
}

