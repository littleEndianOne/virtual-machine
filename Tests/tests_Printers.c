/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tests_Printers.h"
#include <stdint-gcc.h>

#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"
#include "../VM Utility/vm_printers.h"

void RunSet_Printers() {
    printf("\n\n\nTest VM Printers: \n\n");
    mu_run_test(test_Printer_PrintVMState);
    mu_run_test(test_Printer_OpStack);
}

char* test_Printer_PrintVMState() {
    uint8_t program[] = {
        CONSTF8, 2,
        CONSTF8, 4,
        CALL, 9, 0, 2, 1, //Call to next opcode - target low byte, high byte, args count, memory allocation
        CONSTF8, 6,
        CONSTF8, 8,
        CONSTF8, 10,
        CALL, 20, 0, 1, 2, //Call to next opcode - target low byte, high byte, args count, memory allocation        
        CONSTFN1,
        CONSTFN3,
        STORE, 0,
        STORE, 1,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              3, // number type globals to be reserved
                              27, 20); //code size

    vm_Run(vm);

    printf("---Test PrintOpStack---\n");
    PrintThreadState(vm);

    mu_assert("Stack pointer not at expected value!", (vm->sp == 4));
    mu_assert("Error state not as expected!", vm->errorCode == OK);

    vm_Free(vm);
    return 0;
}

char* test_Printer_OpStack() {
    uint8_t program[] = {
        CONSTF8, 2,
        CONSTF8, 4,
        CALL, 9, 0, 2, 1, //Call to next opcode - target low byte, high byte, args count, memory allocation
        CONSTF8, 6,
        CONSTF8, 8,
        CONSTF8, 10,
        CALL, 20, 0, 1, 2, //Call to next opcode - target low byte, high byte, args count, memory allocation        
        CONSTFN1,
        CONSTFN3,
        STORE, 0,
        STORE, 1,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              27, 20); //code size   

    vm_Run(vm);

    printf("---Test PrintOpStack---\n");
    PrintOpStack(vm->opStack, vm->sp);

    mu_assert("Stack pointer not at expected value!", (vm->sp == 4));
    mu_assert("Error state not as expected!", vm->errorCode == OK);

    vm_Free(vm);
    return 0;
}




