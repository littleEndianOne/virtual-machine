#include <stdint-gcc.h>
#include <string.h>
#include "../VM Utility/vm_printers.h"
#include "tests_CALL_RET.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_CALL() {
    printf("\n\n\nTest CALL/RET Operators: \n\n");
    mu_run_test(test_CALL_NoArgs);
    mu_run_test(test_CALL_NoArgPushedBeforeLoad);
    mu_run_test(test_CALL_InvalidArgIndex);
    mu_run_test(test_CALL_NoReturnValue);
    mu_run_test(test_CALL_NumArg);
    mu_run_test(test_CALL_2_NumArg);
    mu_run_test(test_CALL_3_NumArg);
    mu_run_test(test_CALL_3_NumArgReturn);
    mu_run_test(test_CALL_2_Func);
    mu_run_test(test_CALL_MultipleCalls);
    mu_run_test(test_CALL_LocalAllocation);
    mu_run_test(test_RET_NoFunc);
    mu_run_test(test_CALL_NestedCalls);
}

char* test_CALL_NoArgs() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 6, 0, 0, 0, //Function address low byte, high byte, args count, memory allocation
        HALT,
        CONSTF8, 24,
        RET
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              9, //code size
                              20); 
    
    //Get the localScope pointer
    vm_scope* startScope = vm->localScope;
    
    vm_Run(vm);
    
    float expectedResult = 24;
    uint8_t result = vm->opStack[vm->sp].value.float32 == expectedResult; //Compare value on stack to uint representation of float.

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Stack pointer not at 0!", (vm->sp == 0));
    mu_assert("Scope not reset", (vm->localScope == startScope));

    vm_Free(vm);

    return 0;
}

char* test_CALL_InvalidArgIndex() {
    uint8_t program[] = {
        CALL, 6, 0, 1, 0, //Function address low byte, high byte, args count, memory allocation
        HALT,
        LDARG, 1,
        RET,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              9, //code size
                              20); 
    vm_Run(vm);

    mu_assert("Error state not as expected!", vm->errorCode == INVALID_PARAM);
    //mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Stack not empty!", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_CALL_NoArgPushedBeforeLoad() {
    uint8_t program[] = {
        CALL, 6, 0, 1, 0, //Function address low byte, high byte, args count, memory allocation
        HALT,
        LDARG, 0,
        RET,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              9, //code size
                              20); 
    vm_Run(vm);

    mu_assert("Error state not as expected!", vm->errorCode == INVALID_PARAM); //Nothing on the stack to load.
    //mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Stack not empty!", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_CALL_NoReturnValue() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 6, 0, 0, 0, //Function address low byte, high byte, args count, memory allocation
        HALT,
        RET
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              7, //code size
                              20); 
    
    //Get the localScope pointer
    vm_scope* startScope = vm->localScope;
    
    vm_Run(vm);

    //uint8_t error = vm->errorCode;
    //uint8_t opcode =vm->opcode;
    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));  
    mu_assert("Scope not reset", (vm->localScope == startScope));

    vm_Free(vm);

    return 0;
}

char* test_CALL_NumArg() {
    union TypesUnion operandA;
    operandA.float32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CALL, 11, 0, 1, 0, //Function address low byte, high byte, args count, memory allocation
        HALT,
        LDARG, 0,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              14, //code size
                              20); 
    vm_Run(vm);
    //PrintCallStack(vm->localScope);

    //uint8_t error = vm->errorCode;
    uint8_t result = vm->opStack[vm->sp].value.float32 == operandA.float32; //Compare value on stack to uint representation of float.

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Stack pointer not at 1!", (vm->sp == 1));


    vm_Free(vm);
    return 0;
}

char* test_CALL_2_NumArg() {
    union TypesUnion operandA;
    operandA.float32 = 4096;
    union TypesUnion operandB;
    operandB.float32 = 8192;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh,
        operandB.bytes.high,
        CALL, 16, 0, 2, 0, //Function address low byte, high byte, args count, memory allocation
        HALT,
        LDARG, 0,
        LDARG, 1,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              21, 20); //code size
    vm_Run(vm);

    float loadedA = vm->opStack[vm->sp].value.float32;
    float loadedB = vm->opStack[vm->sp - 1].value.float32;

    //RET will place result of subtraction on top of the op stack.
    //Compare value on stack to uint representation of float.
    uint8_t result = (loadedA == operandA.float32);

    result = result & (loadedB == operandB.float32);
    mu_assert("Stack pointer not at 3!", (vm->sp == 3));
    //uint8_t error = vm->errorCode;

    mu_assert("Pushed values don't match values on stack!", result);
    mu_assert("Expected error state OK", vm->errorCode == OK);

    vm_Free(vm);
    return 0;
}

char* test_CALL_3_NumArg() {
    union TypesUnion operandA;
    operandA.float32 = 12203.1;
    union TypesUnion operandB;
    operandB.float32 = 12434.1;
    union TypesUnion operandC;
    operandC.float32 = 89874.1;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh,
        operandB.bytes.high,
        CONSTF, operandC.bytes.low, operandC.bytes.midLow, operandC.bytes.midHigh,
        operandC.bytes.high,
        CALL, 21, 0, 3, 0, //Function address low byte, high byte, args count, memory allocation
        HALT,
        LDARG, 0,
        LDARG, 1,
        LDARG, 2,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              28, 
                              20); //code size
    vm_Run(vm);

    //StackDump(vm);

    float loadedA = vm->opStack[vm->sp].value.float32;
    float loadedB = vm->opStack[vm->sp - 1].value.float32;
    float loadedC = vm->opStack[vm->sp - 2].value.float32;

    //RET will place result of subtraction on top of the op stack.
    //Compare value on stack to uint representation of float.
    uint8_t result = (loadedA == operandA.float32) & (loadedB == operandB.float32) & (loadedC == operandC.float32);


    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at 5!", (vm->sp == 5));
    //uint8_t error = vm->errorCode;

    mu_assert("Pushed values don't match values on stack!", result);

    vm_Free(vm);
    return 0;
}

char* test_CALL_3_NumArgReturn() {
    union TypesUnion operandA;
    operandA.float32 = 12203.1;
    union TypesUnion operandB;
    operandB.float32 = 12434.1;
    union TypesUnion operandC;
    operandC.float32 = 89874.1;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh,
        operandB.bytes.high,
        CONSTF, operandC.bytes.low, operandC.bytes.midLow, operandC.bytes.midHigh,
        operandC.bytes.high,
        CALL, 21, 0, 3, 0, //Function address low byte, high byte, args count, memory allocation
        HALT,
        LDARG, 0,
        LDARG, 1,
        LDARG, 2,
        RET
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              28, 
                              20); //code size
    //Get the localScope pointer
    vm_scope* startScope = vm->localScope;
    
    vm_Run(vm);

    //uint8_t error = vm->errorCode;
    float loadedA = vm->opStack[vm->sp].value.float32;

    //RET will place result of subtraction on top of the op stack.
    //Compare value on stack to uint representation of float.
    uint8_t result = (loadedA == operandA.float32);


    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vm->sp == 0));
    mu_assert("Pushed values don't match values on stack!", result);
    mu_assert("Scope not reset", (vm->localScope == startScope));
    vm_Free(vm);
    return 0;
}

char* test_CALL_2_Func() {
    union TypesUnion operandA;
    operandA.float32 = 4096;
    union TypesUnion operandB;
    operandB.float32 = 8192;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh,
        operandB.bytes.high,
        CALL, 15, 0, 2, 0, //Function address low byte, high byte, args count, memory allocation
        LDARG, 1,
        LDARG, 0,
        CALL, 24, 0, 2, 0,
        LDARG, 0,
        LDARG, 1,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              29, 
                              20); //code size
    vm_Run(vm);

    //StackDump(vm);

    float loadedA = vm->opStack[vm->sp].value.float32;
    float loadedB = vm->opStack[vm->sp - 1].value.float32;

    //RET will place result of subtraction on top of the op stack.
    //Compare value on stack to uint representation of float.
    uint8_t result = (loadedA == operandA.float32);

    result = result & (loadedB == operandB.float32);
    mu_assert("Stack pointer not at expected value!", (vm->sp == 5));
    //uint8_t error = vm->errorCode;

    mu_assert("Pushed values don't match values on stack!", result);
    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("localScope should not be null", (vm->localScope != NULL));
    mu_assert("localScope->prevScope should not null", (vm->localScope->prevScope != NULL));

    vm_Free(vm);
    return 0;
}

char* test_CALL_LocalAllocation() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        HALT,
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              6, 
                              20); //code size
    vm_Run(vm);

    //check memory allocation
    vm_element* ptr = (vm->localScope->localsArray); //Get ptr to store number

    //check count
    uint8_t localsCount = vm->localScope->localsCount;

    //printf("Pointer: %p \n", allocPtr);
    mu_assert("Locals memory not allocated", (ptr != NULL)); //function frame.
    mu_assert("Locals count incorrect", localsCount == 1);
    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stack pointer not empty!", (vm->sp == -1)); //function frame.

    vm_Free(vm);

    return 0;
}

char* test_CALL_MultipleCalls() {
    union TypesUnion operandA;
    operandA.float32 = 255;
    union TypesUnion operandB;
    operandB.float32 = 255;

    uint8_t program[] = {

        //Function -  Add
        LDARG, 0,
        LDARG, 1,
        ADDF, //Return result
        RET,
        // entrypoint - main function
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh,
        operandB.bytes.high,
        CALL, 0, 0, 2, 1, //Call add - Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CALL, 0, 0, 2, 1, //Call add
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              6, // start address of main function
                              0, // number type globals to be reserved
                              33, 20); //code size
    
    //Get the localScope pointer
    vm_scope* startScope = vm->localScope;
    vm_Run(vm);

    //(vm->opStack, vm->sp)    
    //PrintCallStack(vm->localScope);

    uint8_t result = vm->opStack[vm->sp].value.float32 == 765;
    
    
    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stack pointer not at expected value!", (vm->sp == 0));
    mu_assert("Pushed values don't match values on stack!", result);
    mu_assert("Scope not reset", (vm->localScope == startScope));
    
    vm_Free(vm);
    return 0;
}

//Return from the default (init) localScope
//Will return to the beginning of the code and bottom of the stack.
char* test_RET_NoFunc() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTF8, 24,
        RET
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              3, //code size
                              20); 
    
    //Get the localScope pointer
    vm_scope* startScope = vm->localScope;
    vm_Run(vm);
    
    float expectedResult = 24;
    uint8_t result = vm->opStack[vm->sp].value.float32 == expectedResult; //Compare value on stack to uint representation of float.

    mu_assert("Expected error state OK", vm->errorCode == RET_NO_SCOPE);
    mu_assert("Stack pointer not at 0!", (vm->sp == 0));    
    mu_assert("Scope not as expected", (vm->localScope == startScope));
    mu_assert("value on stack does not match expected", result);
    vm_Free(vm);

    return 0;
}

char* test_CALL_NestedCalls() {
    union TypesUnion operandA;
    operandA.float32 = 10;
    union TypesUnion operandB;
    operandB.float32 = 10;

    uint8_t program[] = {

        //Function -  Add
        //Add garbage to be left on stack
        CONSTFN10,
        CONSTFN10,
        LDARG, 0,
        LDARG, 1,
        ADDF, //Return result
        CALL, 13, 0, 1, 0,
        RET,
        //Function - Multiply by Two
        //Add garbage to be left on stack
        CONSTFN9,
        CONSTFN9,
        LDARG, 0,
        CONSTFN2,
        MULF,
        RET,
        // entrypoint - main function
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh,
        operandB.bytes.high,
        CALL, 0, 0, 2, 1, //Call add - Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CALL, 0, 0, 2, 1, //Call add
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              20, // start address of main function
                              0, // number type globals to be reserved
                              46, //code size
                              20); 
    
    //Get the localScope pointer
    vm_scope* startScope = vm->localScope;
    vm_Run(vm);

    uint8_t result = vm->opStack[vm->sp].value.float32 == 100;

    mu_assert("Stack pointer not at expected value!", (vm->sp == 0));
    mu_assert("Pushed value doesn't match value on stack!", result);
    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Scope not reset", (vm->localScope == startScope));
    vm_Free(vm);
    return 0;
}


