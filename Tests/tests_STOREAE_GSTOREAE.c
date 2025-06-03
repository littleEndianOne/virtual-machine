#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "tests_STOREAE_GSTOREAE.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_STOREAE_GSTOREAE() {
    printf("\n\n\nTest STOREAE & GSTOREAE Operators: \n\n");

    mu_run_test(test_GSTOREAE_Normal);  
    mu_run_test(test_GSTOREAE_VariableOutOfBounds);
    mu_run_test(test_GSTOREAE_IndexOutOfBounds);
    mu_run_test(test_GSTOREAE_IncorectType);
    
    mu_run_test(test_STOREAE_Normal);   
    mu_run_test(test_GSTOREAE_NoGlobals);
    mu_run_test(test_STOREAE_NoFuncScope);
}

char* test_GSTOREAE_Normal() {

    uint8_t program[] = {
        // entrypoint - main function
       
        GDARRAY, 14, 0, //Create string allocation with length 14 at address 0. 
        CONSTFN8, //value
        CONSTIN0, //index
        GSTOREAE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              8,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Array allocation length not as expected", vm->globalsArray[0].value.arrayPtr->size == 14);
    mu_assert("Array pointer not set", vm->globalsArray[0].value.arrayPtr != NULL);
    mu_assert("Value not set at address", vm->globalsArray[0].value.arrayPtr->data[0].value.float32 == 8);
    mu_assert("Variable not set to array type", vm->globalsArray[0].type == ARRAY_REF);
    mu_assert("Stack not empty!", (vm->sp == -1));
    vm_Free(vm);
    return 0;
}

char* test_GSTOREAE_VariableOutOfBounds() {

    uint8_t program[] = {
        // entrypoint - main function       
        GDARRAY, 14, 0, //Create string allocation with length 14 at address 0. 
        CONSTFN8, //value
        CONSTIN0, //index
        GSTOREAE, 1,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              8,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == MEM_SEG_FAULT);
    vm_Free(vm);
    return 0;
}

char* test_GSTOREAE_IndexOutOfBounds() {

    uint8_t program[] = {
        // entrypoint - main function       
        GDARRAY, 14, 0, //Create string allocation with length 14 at address 0. 
        CONSTFN8, //value
        CONSTI8, 14, //index out of bounds
        GSTOREAE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              9,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == MEM_SEG_FAULT);
    vm_Free(vm);
    return 0;
}

//Array at address 0 not initialised to array. type == NOT_SET.
char* test_GSTOREAE_IncorectType() {

    uint8_t program[] = {
        // entrypoint - main function       
        CONSTFN8, //value
        CONSTIN0, //index
        GSTOREAE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              8,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == TYPE_ERROR);
    vm_Free(vm);
    return 0;
}

char* test_STOREAE_Normal() {

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DARRAY, 14, 0, //Create string allocation with length 14 at address 0. 
        CONSTFN8, //value
        CONSTIN0, //index
        STOREAE, 0,
        HALT,
        
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              13,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Array allocation length not as expected", vm->localScope->localsArray[0].value.arrayPtr->size == 14);
    mu_assert("Array pointer not set", vm->localScope->localsArray[0].value.arrayPtr != NULL);
    mu_assert("Value not set at address", vm->localScope->localsArray[0].value.arrayPtr->data[0].value.float32 == 8);
    mu_assert("Variable not set to array type", vm->localScope->localsArray[0].type == ARRAY_REF);
    mu_assert("Stack not empty!", (vm->sp == -1));
    vm_Free(vm);
    return 0;
}


char* test_GSTOREAE_NoGlobals() {

    uint8_t program[] = {
        // entrypoint - main function       
        CONSTFN8, //value
        CONSTIN0, //index
        GSTOREAE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              0,        //number type globals to be reserved
                              5,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == MEM_SEG_FAULT);
    vm_Free(vm);
    return 0;
}

char* test_STOREAE_NoFuncScope() {

    uint8_t program[] = {
        // entrypoint - main function       
        CONSTFN8, //value
        CONSTIN0, //index
        STOREAE, 0,
        HALT,
        
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              5,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == MEM_SEG_FAULT);

    vm_Free(vm);
    return 0;
}
