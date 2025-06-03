#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "tests_DARAY_GDARAY.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

/*NOTE: Running these tests in debug mode will reveal any segmentation faults.*/

void RunSet_DARY_GDARAY() {
    printf("\n\n\nTest DARAY & GDARAY Operators: \n\n");

    mu_run_test(test_GDARAY_Normal);
    mu_run_test(test_GDARAY_MaxLengthArray);
    mu_run_test(test_GDARAY_ZeroLengthAllocation);
    mu_run_test(test_DARAY_Normal);
}

char* test_GDARAY_Normal() {

    uint8_t program[] = {
        // entrypoint - main function
       
        GDARRAY, 14, 0, //Create string allocation with length 14 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // number type globals to be reserved
                              4, //code size
                              20); //stack size

    vm_Run(vm);
    
    vm->globalsArray[0].value.arrayPtr->data[0].value.int32 = 10;    
    
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Stack not empty!", (vm->sp == -1));
    mu_assert("Array allocation length not as expected", vm->globalsArray[0].value.arrayPtr->size == 14);
    mu_assert("Array pointer not set", vm->globalsArray[0].value.arrayPtr != NULL);
    mu_assert("Value not set at address", vm->globalsArray[0].value.arrayPtr->data[0].value.int32 == 10);
    mu_assert("Variable not set to array type", vm->globalsArray[0].type == ARRAY_REF);
    vm_Free(vm);
    return 0;
}

char* test_GDARAY_MaxLengthArray() {

    uint8_t program[] = {
        // entrypoint - main function
       
        GDARRAY, 255, 0, //Create string allocation with length 255 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // number type globals to be reserved
                              4, //code size
                              20); //stack size

    vm_Run(vm);
    
    vm->globalsArray[0].value.arrayPtr->data[254].value.int32 = 10;
   
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Stack not empty!", (vm->sp == -1));
    mu_assert("Array allocation length not as expected", vm->globalsArray[0].value.arrayPtr->size == 255);
    mu_assert("Array pointer not set", vm->globalsArray[0].value.arrayPtr != NULL);
    mu_assert("Value not set at highest address", vm->globalsArray[0].value.arrayPtr->data[254].value.int32 == 10);
    mu_assert("Variable not set to array type", vm->globalsArray[0].type == ARRAY_REF);
    vm_Free(vm);
    return 0;
}

char* test_GDARAY_ZeroLengthAllocation() {

    uint8_t program[] = {
        // entrypoint - main function
       
        GDARRAY, 0, 0, //Create string allocation with length 255 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // number type globals to be reserved
                              4, //code size
                              20); //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == INVALID_PARAM);
    mu_assert("Stack not empty!", (vm->sp == -1));
    
    vm_Free(vm);
    return 0;
}

char* test_DARAY_Normal() {

    uint8_t program[] = {
        // entrypoint - main function
        
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DARRAY, 14, 0, //Create string allocation with length 14 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // number type globals to be reserved
                              9, //code size
                              20); //stack size

    vm_Run(vm);
    
    vm->localScope->localsArray[0].value.arrayPtr->data[0].value.int32 = 10; 
    
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Stack not empty!", (vm->sp == -1));
    mu_assert("Array allocation length not as expected", vm->localScope->localsArray[0].value.arrayPtr->size == 14);
    mu_assert("Array pointer not set", vm->localScope->localsArray[0].value.arrayPtr != NULL);
    mu_assert("Value not set at address", vm->localScope->localsArray[0].value.arrayPtr->data[0].value.int32 == 10);
    mu_assert("Variable not set to array type", vm->localScope->localsArray[0].type == ARRAY_REF);
    vm_Free(vm);
    return 0;
}





