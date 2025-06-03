#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "tests_CONV.h"
#include "tests_DSTR_GDSTR.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_DSTR_GDSTR() {
    printf("\n\n\nTest DSTR & GDSTR Operators: \n\n");

    mu_run_test(test_DSTR_TypicalString);
    mu_run_test(test_DSTR_MaxLengthString);
    mu_run_test(test_DSTR_ZeroLengthAllocation);
    mu_run_test(test_DSTR_MinLengthString);
    mu_run_test(test_GDSTR_TypicalString);
    mu_run_test(test_DSTR_NoFuncScope);
}

char* test_DSTR_TypicalString() {

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 14, 0, //Create string allocation with length 14 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              9, //code size
                              20); 

    vm_Run(vm);

    char exptectedString[] = "\0";
    //printf("String pushed: %s \n", vm->opStack->value.strPtr);

    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->localScope->localsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->localScope->localsArray[0].value.strRef.allocLength == 14);

    vm_Free(vm);
    return 0;
}

char* test_DSTR_MaxLengthString() {

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 255, 0, //Create string allocation with length 14 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              9, //code size
                              20); 

    vm_Run(vm);

    char exptectedString[] = "\0";
    //printf("String pushed: %s \n", vm->opStack->value.strPtr);

    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->localScope->localsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->localScope->localsArray[0].value.strRef.allocLength == 255);

    vm_Free(vm);
    return 0;
}

char* test_DSTR_ZeroLengthAllocation() {

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 0, 0, //Create string allocation with length 14 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              9, //code size
                              20); 

    vm_Run(vm);

    mu_assert("Error state not OK", vm->errorCode == INVALID_PARAM);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory


    vm_Free(vm);
    return 0;
}

char* test_DSTR_MinLengthString() {

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 1, 0, //Create string allocation with length 14 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              9, //code size
                              0); 

    vm_Run(vm);

    char exptectedString[] = "\0";
    //printf("String pushed: %s \n", vm->opStack->value.strPtr);

    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->localScope->localsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->localScope->localsArray[0].value.strRef.allocLength == 1);

    vm_Free(vm);
    return 0;
}

char* test_GDSTR_TypicalString() {

    uint8_t program[] = {
        // entrypoint - main function        
        GDSTR, 14, 0, //Create string allocation with length 14 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // number type globals to be reserved
                              4, //code size
                              20); 

    vm_Run(vm);

    char exptectedString[] = "\0";
    //printf("String pushed: %s \n", vm->opStack->value.strPtr);

    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->globalsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->globalsArray[0].value.strRef.allocLength == 14);

    vm_Free(vm);
    return 0;
}

char* test_DSTR_NoFuncScope() {

    uint8_t program[] = {
        // entrypoint - main function       
        DSTR, 14, 0, //Create string allocation with length 14 at address 0.  
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              4, //code size
                              20); 

    vm_Run(vm);

    mu_assert("Error state not OK", vm->errorCode == MEM_SEG_FAULT);

    vm_Free(vm);
    return 0;
}



