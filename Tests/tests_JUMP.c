#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_JUMP.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_JUMP() {
    printf("\n\n\nTest JUMP Operators: \n\n");
    mu_run_test(test_JMP);
    mu_run_test(test_JMPT_TRUE);
    mu_run_test(test_JMPT_FALSE);
    mu_run_test(test_JMPF_TRUE);
    mu_run_test(test_JMPF_FALSE);
    mu_run_test(test_JMP_JumpToAddressOutOfBounds);
    mu_run_test(test_JMPT_TRUE_JumpToAddressOutOfBounds);
    mu_run_test(test_JMPF_FALSE_JumpToAddressOutOfBounds);
    mu_run_test(test_JMP_JumpTo16Address);
    mu_run_test(test_JMPT_FALSE_Float);
    mu_run_test(test_JMPT_TRUE_Float);
}

char* test_JMP() {
    uint8_t program[] ={
        // entrypoint - main function
        CONSTI8, 64,
        JMP, 0x7, 0x0, //Jump to byte 5 (0xFE)
        0xFE,
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              9, 
                              20);
   
    //step the program to complete execution of the operator under test.
    vm_StartDebug(vm);   
    vm_DebugRunFor(vm, 2);

    uint8_t result = vm->pc == 7; //pc should be set to byte 5 of code.
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vm->sp == 0));
    mu_assert("Program counter (pc) not set to expected value", result);

    vm_Free(vm);
    return 0;
}

char* test_JMPT_TRUE() {

    uint8_t program[] ={
        // entrypoint - main function
        CONSTI8, 1, //Push true value
        JMPT, 0x7, 0x0, //Jump to code byte 5 if true on top of stack
        0xFE,
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              9, 
                              20);

    //step the program to complete execution of the operator under test.    
    vm_StartDebug(vm);
    vm_DebugRunFor(vm, 2);

    uint8_t result = vm->pc == 7;
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));
    mu_assert("Program counter (pc) not set to expected value", result);

    vm_Free(vm);
    return 0;
}

char* test_JMPT_FALSE() {

    uint8_t program[] ={
        // entrypoint - main function
        CONSTI8, 0, //Push false value
        JMPT, 0x7, 0x0, //Jump to code byte 5 if true on top of stack
        0xFE,
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              9, 
                              20);

    //step the program to complete execution of the operator under test.
    vm_StartDebug(vm);    
    vm_DebugRunFor(vm, 2);

    uint8_t result = vm->pc == 5;
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));
    mu_assert("Program counter (pc) not set to expected value", result);

    vm_Free(vm);
    return 0;
}

char* test_JMPF_TRUE() {

    uint8_t program[] ={
        // entrypoint - main function
        CONSTI8, 1, //Push true value
        JMPF, 0x7, 0x0, //Jump to code byte 5 if false on top of stack
        0xFE,
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              9, 
                              20);

    //step the program to complete execution of the operator under test.
    vm_StartDebug(vm);
    vm_DebugRunFor(vm, 2);

    uint8_t result = vm->pc == 5;
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));
    mu_assert("Program counter (pc) not set to expected value", result);

    vm_Free(vm);
    return 0;
}

char* test_JMPF_FALSE() {

    uint8_t program[] ={
        // entrypoint - main function
        CONSTI8, 0, //Push true value
        JMPF, 0x7, 0x0, //Jump to code byte 5 if true on top of stack
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              9, 
                              20);
    //run(vm);

    //step the program to complete execution of the operator under test.
    vm_StartDebug(vm);    
    vm_DebugRunFor(vm, 2);

    uint8_t result = vm->pc == 7;
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));
    mu_assert("Program counter (pc) not set to expected value", result);

    vm_Free(vm);
    return 0;
}

char* test_JMP_JumpToAddressOutOfBounds() {

    uint8_t program[] ={
        // entrypoint - main function
        CONSTI8, 64,
        JMP, 0x32, 0x0, //Jump to byte 10 (invalid)
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              7,  //Code size
                              20); //Stack size
    vm_Run(vm);

    mu_assert("Error state not INVALID_CODE_ADDRESS", vm->errorCode == PROG_SEG_FAULT);
    mu_assert("Run state not FAULT", vm->runState == FAULT);
    mu_assert("Stack pointer not at 0!", (vm->sp == 0));
    mu_assert("Current opcode not JMP", vm->opcode == JMP);

    vm_Free(vm);
    return 0;
}

char* test_JMPT_TRUE_JumpToAddressOutOfBounds() {
    uint8_t program[] ={
        // entrypoint - main function
        CONSTI8, 1, //Push true value
        JMPT, 0x32, 0x0, //Jump to code byte 5 if true on top of stack
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              7, 
                              20);
    vm_Run(vm);

    mu_assert("Error state not FAULT", vm->runState == FAULT);
    mu_assert("Error state not INVALID_CODE_ADDRESS", vm->errorCode == PROG_SEG_FAULT);
    mu_assert("Stack pointer not at 0!", (vm->sp == -1));
    mu_assert("Current opcode not JMPF", vm->opcode == JMPT);

    vm_Free(vm);
    return 0;
}

char* test_JMPF_FALSE_JumpToAddressOutOfBounds() {
    uint8_t program[] ={
        // entrypoint - main function
        CONSTI8, 0, //Push true value
        JMPF, 0x032, 0x32, //Jump to code byte 5 if true on top of stack
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              7, 
                              20);
    vm_Run(vm);

    mu_assert("Error state not FAULT", vm->runState == FAULT);
    mu_assert("Error state not INVALID_CODE_ADDRESS", vm->errorCode == PROG_SEG_FAULT);
    mu_assert("Stack pointer not at 0!", (vm->sp == -1));
    mu_assert("Current opcode not JMPF", vm->opcode == JMPF);

    vm_Free(vm);
    return 0;
}

//Jump to high address 1024 and execute instruction.

char* test_JMP_JumpTo16Address() {
    uint16_t fillCodeTo = 1024;
    uint16_t codeLength = fillCodeTo + 3;

    char program[codeLength];
    int i;

    //generate large program length

    program[0] = JMP;
    program[1] = 0x00;
    program[2] = 0x04; //1024

    //Fill the code up to 1024 with 0xFF starting at 3
    for (i = 3; i < fillCodeTo; i++) {
        program[i] = 0xFF;
    }
    //printf("%i", i);
    program[1024] = CONSTF8; //Instruction at 1024
    program[1025] = 32;
    program[1026] = HALT; //Add HALT opcode to the end.



    //printProgramAsDec(program, codeLength);
    //printf("\n");
    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents((uint8_t*) program, // program to execute
                              0, // start address of main function
                              0,
                              codeLength, 
                              20);
    vm_Run(vm);

    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Run state not HALT", vm->runState == HALTED);
    mu_assert("Stack pointer not at 0!", (vm->sp == 0));
    mu_assert("Incorrect value pushed onto stack!", vm->opStack[vm->sp].value.float32 == 32.0);
    //mu_assert("Program counter (pc) not set to expected value", vm->pc == );

    vm_Free(vm);
    return 0;
}

//Test using float value for boolean condition
char* test_JMPT_TRUE_Float() {

    uint8_t program[] ={
        // entrypoint - main function
        CONSTF8, 1, //Push true value
        JMPT, 0x7, 0x0, //Jump to code byte 5 if true on top of stack
        0xFE,
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              9, 
                              20);

    //step the program to complete execution of the operator under test.    
    vm_StartDebug(vm);    
    vm_DebugRunFor(vm, 2);

    uint8_t result = vm->pc == 7;
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));
    mu_assert("Program counter (pc) not set to expected value", result);

    vm_Free(vm);
    return 0;
}

//Test using float value for boolean condition
char* test_JMPT_FALSE_Float() {

    uint8_t program[] ={
        // entrypoint - main function
        CONSTF8, 0, //Push true value
        JMPT, 0x7, 0x0, //Jump to code byte 5 if true on top of stack
        0xFE,
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              9, 
                              20);
    //run(vm);

    //step the program to complete execution of the operator under test.    
    vm_StartDebug(vm);    
    vm_DebugRunFor(vm, 2);

    uint8_t result = vm->pc == 5;
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));
    mu_assert("Program counter (pc) not set to expected value", result);

    vm_Free(vm);
    return 0;
}

//Test using float value for boolean condition
char* test_JMPT_TRUE_ComplexFloat() {
    vm_value value;
    value.float32 = -0.999;
    uint8_t program[] ={
        // entrypoint - main function
        CONSTF, value.bytes.low, value.bytes.midLow, value.bytes.midHigh, value.bytes.midHigh, //Push value
        JMPT, 0x10, 0x0, //Jump to code byte 5 if true on top of stack
        0xFE,
        0xFE,
        0xFF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              12, 
                              20);

    //step the program to complete execution of the operator under test.    
    vm_StartDebug(vm);    
    vm_DebugRunFor(vm, 2);

    uint8_t result = vm->pc == 10;
    mu_assert("Error state not OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));
    mu_assert("Program counter (pc) not set to expected value", result);

    vm_Free(vm);
    return 0;
}
