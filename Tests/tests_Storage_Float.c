#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "tests_Storage_Float.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_STORE_LOAD_Float() {
    printf("\n\n\nTest Storage Operators for Float type: \n\n");
    mu_run_test(test_STORE_1_Float);
    mu_run_test(test_STORE_2_Float);
    mu_run_test(test_STORE_2_Float_AddressOutOfBounds);

    mu_run_test(test_LOAD_1_Flaot);
    mu_run_test(test_LOAD_3_Float);

    mu_run_test(test_LOAD_Float_AddressOutOfBounds);
    mu_run_test(test_LOAD_Float_LoadBeforeStore);
    mu_run_test(test_STORE_Float_StringAlreadyStoredAtAddress);
    mu_run_test(test_STORE_LOAD_Float_HighestAddressableIndex);
    
    mu_run_test(test_GSTORE);
    mu_run_test(test_GLOAD);
}

char* test_STORE_1_Float() {
    union TypesUnion operandA;
    operandA.float32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              13, //code size
                              20); 
    
    vm_Run(vm);

    //StackDump(vm);

    float stored = vm->localScope->localsArray[0].value.float32;

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stored value does not match pushed value", stored == operandA.float32);
    mu_assert("Stack pointer not empty!", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_STORE_2_Float() {
    union TypesUnion operandA;
    operandA.float32 = 2048;
    union TypesUnion operandB;
    operandB.float32 = 1024;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        CONSTF,  operandA.bytes.low, 
                operandA.bytes.midLow, 
                operandA.bytes.midHigh, 
                operandA.bytes.high,
        STORE, 0,
        CONSTF,  operandB.bytes.low, 
                operandB.bytes.midLow, 
                operandB.bytes.midHigh, 
                operandB.bytes.high,
        STORE, 1,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              20, //code size
                              20); //stack size 
    vm_Run(vm);

    //PrintCallStack(vm->localScope);

    float storeda = vm->localScope->localsArray[0].value.float32;
    float storedb = vm->localScope->localsArray[1].value.float32;

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stored value does not match pushed value", storeda == operandA.float32);
    mu_assert("Stored value does not match pushed value", storedb == operandB.float32);
    mu_assert("Stack not empty", vm->sp == -1);

    vm_Free(vm);
    return 0;
}

char* test_STORE_2_Float_AddressOutOfBounds() {
    union TypesUnion operandA;
    operandA.float32 = 2048;
    union TypesUnion operandB;
    operandB.float32 = 1024;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh, operandB.bytes.high,
        STORE, 1,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              20, //code size
                              20); //Stack size

    vm_Run(vm);

    float storeda = vm->localScope->localsArray[0].value.float32;

    mu_assert("Expected error state MEM_SEG_FAULT not set", vm->errorCode == MEM_SEG_FAULT);
    mu_assert("Stored value does not match pushed value", storeda == operandA.float32);
    mu_assert("Operand stack not in expected state", vm->sp == 0);

    vm_Free(vm);
    return 0;
}

char* test_LOAD_1_Flaot() {
    union TypesUnion operandA;
    operandA.float32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        LOAD, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              15, //code size
                              20); //Stack size

    vm_Run(vm);
    //StackDump(vm);

    //float stored = vm->funcScope->locals[0].value.float32;
    float loaded = vm->opStack[0].value.float32; //Get value from top of stack.
    float stored = vm->localScope->localsArray[0].value.float32;

    //uint8_t error = vm->errorCode;
    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Value not stored correctly", stored == operandA.float32);
    mu_assert("Loaded value not on stack!", (vm->sp == 0));
    mu_assert("Loaded value does not match pushed value", loaded == operandA.float32);

    vm_Free(vm);
    return 0;
}

char* test_LOAD_3_Float() {
    vm_value operandA;
    operandA.float32 = 33.333;
    vm_value operandB;
    operandB.float32 = 99.99;
    vm_value operandC;
    operandC.float32 = 1000.111;


    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 3, //Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh, operandB.bytes.high,
        STORE, 1,
        CONSTF, operandC.bytes.low, operandC.bytes.midLow, operandC.bytes.midHigh, operandC.bytes.high,
        STORE, 2,
        LOAD, 2,
        LOAD, 1,
        LOAD, 0,
        HALT //stop probram before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              3, // number type globals to be reserved
                              33, //code size
                              20); //Stack size

    vm_Run(vm);

    //StackDump(vm);

    float loadedA = vm->opStack[2].value.float32; //Get value from top of stack.
    float loadedB = vm->opStack[1].value.float32; //Get value from top of stack.
    float loadedC = vm->opStack[0].value.float32; //Get value from top of stack.

    float storedA = vm->localScope->localsArray[0].value.float32;
    float storedB = vm->localScope->localsArray[1].value.float32;
    float storedC = vm->localScope->localsArray[2].value.float32;

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Loaded values not on stack!", (vm->sp == 2));
    mu_assert("Value not stored correctly", (storedA == operandA.float32) & (storedB == operandB.float32) & (storedC == operandC.float32));
    mu_assert("Loaded values do not match pushed values", (loadedA == operandA.float32) & (loadedB == operandB.float32) & (loadedC == operandC.float32));


    vm_Free(vm);
    return 0;
}

char* test_LOAD_Float_AddressOutOfBounds() {
    vm_value operandA;
    operandA.float32 = 33.333;
    vm_value operandB;
    operandB.float32 = 99.99;
    vm_value operandC;
    operandC.float32 = 1000.111;


    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 3, //Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh, operandB.bytes.high,
        STORE, 1,
        CONSTF, operandC.bytes.low, operandC.bytes.midLow, operandC.bytes.midHigh, operandC.bytes.high,
        STORE, 2,
        LOAD, 2,
        LOAD, 1,
        LOAD, 3,
        HALT //stop probram before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              33, //code size
                              20); //Stack size

    vm_Run(vm);

    //StackDump(vm);

    float loadedB = vm->opStack[1].value.float32; //Get value from top of stack.
    float loadedC = vm->opStack[0].value.float32; //Get value from top of stack.

    float storedA = vm->localScope->localsArray[0].value.float32;
    float storedB = vm->localScope->localsArray[1].value.float32;
    float storedC = vm->localScope->localsArray[2].value.float32;

    mu_assert("Expected error state MEM_SEG_FAULT not set", vm->errorCode == MEM_SEG_FAULT);
    mu_assert("Loaded values not on stack!", (vm->sp == 1));
    mu_assert("Values not stored correctly", (storedA == operandA.float32) & (storedB == operandB.float32) & (storedC == operandC.float32));
    mu_assert("Loaded values do not match pushed values", (loadedB == operandB.float32) & (loadedC == operandC.float32));

    vm_Free(vm);
    return 0;
}

char* test_LOAD_Float_LoadBeforeStore() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        LOAD, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              8, //code size
                              20); //Stack size

    vm_Run(vm);
    //StackDump(vm);

    mu_assert("Expected error state TYPE_NOT_SET not set", vm->errorCode == OK);
    mu_assert("Stack not empty!", (vm->sp == 0));
    mu_assert("Value not on stack!", (vm->sp == 0));
    mu_assert("Expected type not on stack", (vm->opStack[0].type == NONE));


    vm_Free(vm);
    return 0;
}

char* test_STORE_Float_StringAlreadyStoredAtAddress() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0,
        CONSTF8, 128,
        STORE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              21, //code size
                              20); //Stack size
    vm_Run(vm);

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;
    char* expected = "hello";

    mu_assert("Run state not as expected", vm->runState == FAULT);
    mu_assert("Expected error state not as expected", vm->errorCode == TYPE_ERROR);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 6);
    mu_assert("Stack not empty", (vm->sp == -1));


    vm_Free(vm);
    return 0;
}

char* test_STORE_LOAD_Float_HighestAddressableIndex() {
    union TypesUnion operandA;
    operandA.float32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 255, //Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 254,
        LOAD, 254,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              15, //code size
                              20); //Stack size

    vm_Run(vm);
    //StackDump(vm);

    //float stored = vm->funcScope->locals[0].value.float32;
    float loaded = vm->opStack[0].value.float32; //Get value from top of stack.
    float stored = vm->localScope->localsArray[254].value.float32;

    mu_assert("Not Expected error state", vm->errorCode == OK);
    mu_assert("Value not stored correctly", stored == operandA.float32);
    mu_assert("Loaded value not on stack!", (vm->sp == 0));
    mu_assert("Loaded value does not match pushed value", loaded == operandA.float32);

    vm_Free(vm);
    return 0;
}

char* test_GSTORE() {
    union TypesUnion operandA;
    operandA.float32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        GSTORE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // number type globals to be reserved
                              13, 20); //code size

    vm_Run(vm);

    //StackDump(vm);

    float stored = vm->globalsArray[0].value.float32;

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stored value does not match pushed value", stored == operandA.float32);
    mu_assert("Stack pointer not empty!", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_GLOAD() {
    union TypesUnion operandA;
    operandA.float32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        GSTORE, 0,
        GLOAD, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // number type globals to be reserved
                              15, 20); //code size

    vm_Run(vm);
    //StackDump(vm);

    //float stored = vm->funcScope->locals[0].value.float32;
    float loaded = vm->opStack[0].value.float32; //Get value from top of stack.
    float stored = vm->globalsArray[0].value.float32;

    //uint8_t error = vm->errorCode;
    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Value not stored correctly", stored == operandA.float32);
    mu_assert("Loaded value not on stack!", (vm->sp == 0));
    mu_assert("Loaded value does not match pushed value", loaded == operandA.float32);

    vm_Free(vm);
    return 0;
}
