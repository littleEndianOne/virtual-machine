#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "tests_STRLIT.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_STRLIT() {
    printf("\n\n\nTest STRLIT Operator: \n\n");

    mu_run_test(test_STRLIT_MultiCharString);
    mu_run_test(test_STRLIT_MaxLengthString);
    mu_run_test(test_STRLIT_EmptyString);
    mu_run_test(test_STRLIT_StringLengthShort);
    mu_run_test(test_STRLIT_StringLengthLong);
    mu_run_test(test_STRLIT_MissingNullTerm);
}

char* test_STRLIT_MultiCharString() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              9, 20);
    vm_Run(vm);

    //uint8_t errorCode = vm->errorCode;
    //opcode finalOpCode = vm->opcode;
    //reference on stack
    mu_assert("String not on stack!", (vm->sp == 0));
    //string stored in allocated memory
    mu_assert("String start index wrong!", vm->opStack[0].value.strLit.address == 2);
    mu_assert("String length wrong!", vm->opStack[0].value.strLit.length == 5);
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Run state not HALTED", vm->runState == HALTED);
    //printf("String pushed: %s \n", vm->strStack->strPtr);


    vm_Free(vm);
    return 0;
}

char* test_STRLIT_MaxLengthString() {
    uint8_t stringLength = 254; //Subtract 1 for \0
    uint16_t codeLength = stringLength + 4; // STRL, 254, ...string... , \0, HALT
    //Construct byte code
    uint8_t program[codeLength];
    char expectedString[stringLength + 1];
    program[0] = STRLIT;
    program[1] = stringLength;
    uint16_t i;

    for (i = 0; i < stringLength; i++) {
        program[i + 2] = 'a'; //Ofset by two for opcode at 0 and length at 1.
        expectedString[i] = 'a'; //construct string for comparison.
    }

    expectedString[i] = '\0'; //use incremented i value from above.
    program[i + 2] = '\0';

    program[++i + 2] = HALT;

    //printProgramAsChar(program, codeLength);
    //printf("\n");
    //printProgramAsDec(program, codeLength);
    //printf("\n");
    //printf(expectedString);

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents((uint8_t*) program, // program to execute
                              0, // start address of main function
                              0,
                              codeLength, 20);
    vm_Run(vm);

    //uint8_t errorCode = vm->errorCode;

    mu_assert("Test string not constructed correctly",
              strlen(expectedString) == stringLength); //Ensure that string was constructued correctly
    mu_assert("Operand not on stack", (vm->sp == 0));
    mu_assert("String start index wrong!", vm->opStack[0].value.strLit.address == 2);
    mu_assert("String length wrong!", vm->opStack[0].value.strLit.length == 254);
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Run state not OK", vm->runState == HALTED);

    vm_Free(vm);

    return 0;
}

char* test_STRLIT_EmptyString() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Poped lowest to highest (litle endian)
        STRLIT, 0, '\0',
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              4, 20);
    vm_Run(vm);

    //char ptr on stack
    mu_assert("String reference not on stack!", (vm->sp == 0));
    //string stored in allocated memory
    mu_assert("String start index wrong!", vm->opStack[0].value.strLit.address == 2);
    mu_assert("String length wrong!", vm->opStack[0].value.strLit.length == 0);
    mu_assert("Error state not OK", vm->errorCode == OK);

    vm_Free(vm);
    return 0;

}

char* test_STRLIT_StringLengthShort() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Poped lowest to highest (litle endian)
        STRLIT, 4, 'h', 'e', 'l', 'l', 'o', '\0',
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              8, 20);
    vm_Run(vm);

    //uint8_t errorCode = vm->errorCode;
    //opcode finalOpCode = vm->opcode;
    //reference on stack
    mu_assert("Stack not empty", (vm->sp == -1));
    mu_assert("Error state not as expected", vm->errorCode == INVALID_STRING);
    mu_assert("Run state not FAULT", vm->runState == FAULT);
    //printf("String pushed: %s \n", vm->strStack->strPtr);


    vm_Free(vm);
    return 0;
}

char* test_STRLIT_StringLengthLong() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Poped lowest to highest (litle endian)
        STRLIT, 8, 'h', 'e', 'l', 'l', 'o', '\0',
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              9, 20);
    vm_Run(vm);

    //uint8_t errorCode = vm->errorCode;
    //opcode finalOpCode = vm->opcode;
    //reference on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("Error state shoould be INVALID_CODE_ADDRESS!", vm->errorCode == PROG_SEG_FAULT);
    mu_assert("Run state not FAULT", vm->runState == FAULT);
    mu_assert("Failure not at expected byte!", vm->opcode == STRLIT);
    //printf("String pushed: %s \n", vm->strStack->strPtr);


    vm_Free(vm);
    return 0;
}

char* test_STRLIT_MissingNullTerm() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Poped lowest to highest (litle endian)
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o',
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              8, 20);
    vm_Run(vm);

    //uint8_t errorCode = vm->errorCode;
    //vm_opcode finalOpCode = vm->opcode;
    mu_assert("Error state not as expected", vm->errorCode == INVALID_STRING);
    mu_assert("Stack not empty!", (vm->sp == -1));
    mu_assert("Run state not FAULT", vm->runState == FAULT);
    //printf("String pushed: %s \n", vm->strStack->strPtr);


    vm_Free(vm);
    return 0;
}

