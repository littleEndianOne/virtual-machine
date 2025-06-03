#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "tests_LOADAE_GLOADAE.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_LOADAE_GLOADAE() {
    printf("\n\n\nTest LOADAE & GLOADAE Operators: \n\n");

    mu_run_test(test_GLOADAE_Normal);  
    mu_run_test(test_GLOADAE_VariableOutOfBounds);
    mu_run_test(test_GLOADAE_IndexOutOfBounds);
    mu_run_test(test_GLOADAE_ElementTypeNotSet);
    mu_run_test(test_GLOADAE_ArrayNotDeclared);
    mu_run_test(test_LOADAE_Normal);   
    
    mu_run_test(test_GLOADAE_NoGlobals);
    mu_run_test(test_LOADAE_NoFuncScope);
}

char* test_GLOADAE_Normal() {

    uint8_t program[] = {
        // entrypoint - main function       
        GDARRAY, 14, 0, //Create string allocation with length 14 at address 0. 
        CONSTFN8, //value
        CONSTIN0, //index
        GSTOREAE, 0,
        CONSTIN0,
        GLOADAE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              11,       //code size
                              20);      //stack size
    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Value not on stack!", (vm->sp == 0));
    mu_assert("Expected value not on stack", (vm->opStack[0].value.float32 == 8));
    vm_Free(vm);
    return 0;
}

char* test_GLOADAE_VariableOutOfBounds() {

    uint8_t program[] = {
        // entrypoint - main function
       
        GDARRAY, 14, 0, //Create string allocation with length 14 at address 0. 
        CONSTFN8, //value
        CONSTIN0, //index
        GSTOREAE, 0,
        CONSTIN0,
        GLOADAE, 1,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              11,       //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == MEM_SEG_FAULT);
    vm_Free(vm);
    return 0;
}


char* test_GLOADAE_IndexOutOfBounds() {
    uint8_t program[] = {
        // entrypoint - main function       
        GDARRAY, 14, 0, //Create string allocation with length 14 at address 0. 
        CONSTFN8, //value
        CONSTIN0, //index
        GSTOREAE, 0,
        CONSTI8, 14, 
        GLOADAE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              12,       //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == MEM_SEG_FAULT);
    vm_Free(vm);
    return 0;    
}

//Array at address 0 not initialised to array. type == NOT_SET.
char* test_GLOADAE_ElementTypeNotSet() {
    uint8_t program[] = {
        // entrypoint - main function       
        GDARRAY, 14, 0, //Create string allocation with length 14 at address 0. 
        CONSTI8, 0, 
        GLOADAE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              8,       //code size
                              16);      //stack size
    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Value not on stack!", (vm->sp == 0));
    mu_assert("Expected value not on stack", (vm->opStack[0].type == NONE));
    
    vm_Free(vm);
    return 0;    
}

//Variable at address 0 not initialised to array type. type == NOT_SET.
char* test_GLOADAE_ArrayNotDeclared() {
    uint8_t program[] = {
        // entrypoint - main function       
        CONSTI8, 0, 
        GLOADAE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              5,       //code size
                              16);      //stack size
    vm_Run(vm);
    
    mu_assert("Error state not as expected", vm->errorCode == TYPE_ERROR);
    vm_Free(vm);
    return 0;    
}

char* test_LOADAE_Normal() {

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 6, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        HALT,
        DARRAY, 14, 0, //Create string allocation with length 14 at address 0. 
        CONSTFN8, //value
        CONSTIN0, //index
        STOREAE, 0,
        CONSTIN0,
        LOADAE, 0,
        RET
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              1,        //number type globals to be reserved
                              17,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("No value on stack!", (vm->sp == 0));
    mu_assert("Expected value not on stack", (vm->opStack[0].value.float32 == 8));
    vm_Free(vm);
    return 0;
}

char* test_GLOADAE_NoGlobals() {

    uint8_t program[] = {
        CONSTIN0,
        GLOADAE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              0,        //number type globals to be reserved
                              4,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == MEM_SEG_FAULT);
    vm_Free(vm);
    return 0;
}

char* test_LOADAE_NoFuncScope() {

    uint8_t program[] = {
        // entrypoint - main function         
        CONSTIN0,
        LOADAE, 0,
        RET
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program,  //program to execute
                              0,        //start address of main function
                              0,        //number type globals to be reserved
                              4,        //code size
                              20);      //stack size

    vm_Run(vm);
       
    mu_assert("Error state not as expected", vm->errorCode == MEM_SEG_FAULT);
    vm_Free(vm);
    return 0;
}
