#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_stack_operations.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_StackOperations() {
    printf("\n\n\nTest Stack Ops: \n\n");
    mu_run_test(test_POP);
    mu_run_test(test_StackOverflow);
    mu_run_test(test_StackUnderun);
}

char* test_POP() {
    uint8_t program[] ={
        // entry point - main function
        CONSTF8, 128,
        CONSTF8, 64,
        CONSTF8, 32,
        POP,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              8, 20);
    //run(vm);

    //step the program to complete execution of the operator under test.
    vm_StartDebug(vm);
    vm_DebugStep(vm);
    vm_DebugStep(vm);

    mu_assert("Stack pointer not at 1!", (vm->sp == 1));
    mu_assert("Error state not OK", vm->errorCode == OK);

    vm_Free(vm);

    return 0;
}

char* test_StackOverflow() {
    uint16_t stackSize = 40;
    uint8_t stackOverflowCount = stackSize + 1; //One more than size of stack.
    //opcode and operand for each stack push. +1 for HALT opcode.
    uint32_t codeLength = stackOverflowCount * 2 + 1;
    //printf("%i", codeLength);
    char program[codeLength];
    int i;

    for (i = 0; i < stackOverflowCount; i++) {
        program[i * 2] = CONSTF8;
        program[i * 2 + 1] = 97; //ASCII for a
    }
    //printf("%i", i);
    program[i * 2] = HALT; //Add HALT opcode to the end.
    //printProgramAsDec(program, codeLength);
    //printf("\n");
    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents((uint8_t*) program, // program to execute
                              0, // start address of main function
                              0,
                              codeLength, 
                              stackSize);
    vm_Run(vm);
    //printf("\n");
    //uint8_t estate = vm->errorCode;
    mu_assert("Error state not STACK_OVERFLOW", vm->errorCode == STACK_OVERFLOW);
    mu_assert("Stack pointer not at top of stack!", (vm->sp == stackSize)); //String reference will be popped from stack.
    mu_assert("Run state not FAULT", vm->runState == FAULT);

    vm_Free(vm);

    return 0;
}

char* test_StackUnderun() {
    //Brake down int into bytes.

    union TypesUnion pushValue;
    pushValue.float32 = 1024;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (litle endian)
        CONSTF, pushValue.bytes.low, pushValue.bytes.midLow, pushValue.bytes.midHigh,
        pushValue.bytes.high,
        ADDF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              12, 20);
    vm_Run(vm);

    mu_assert("Error state not STACK_UNDERRUN", vm->errorCode == STACK_UNDERRUN);
    mu_assert("Run state not FAULT", vm->runState == FAULT);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));

    vm_Free(vm);

    return 0;
}

