#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "tests_Storage_Int.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_STORE_LOAD_Int() {
    printf("\n\n\nTest Storage Operators for Integer type: \n\n");
    mu_run_test(test_STORE_1_Int);
    mu_run_test(test_STORE_2_Int);
    mu_run_test(test_STORE_2_Int_AddressOutOfBounds);

    mu_run_test(test_LOAD_1_Int);
    mu_run_test(test_LOAD_3_Int);

    mu_run_test(test_LOAD_Int_AddressOutOfBounds);
    mu_run_test(test_LOAD_Int_LoadBeforeStore);
    mu_run_test(test_STORE_Int_StringAlreadyStoredAtAddress);
    mu_run_test(test_STORE_LOAD_Int_HighestAddressableIndex);
    
    mu_run_test(test_GSTORE_1_Int);
    mu_run_test(test_GLOAD_1_Int);
    
    mu_run_test(test_GLOAD_NoGlobalsDeclared);
    mu_run_test(test_STORE_NoLocalsDeclared);
    mu_run_test(test_LOAD_NoLocalsDeclared);
    
    mu_run_test(test_STORE_NoFunctionScope);
    mu_run_test(test_LOAD_NoFunctionScope);   
}

char* test_STORE_1_Int() {
    union TypesUnion operandA;
    operandA.int32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
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

    int stored = vm->localScope->localsArray[0].value.int32;

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stored value does not match pushed value", stored == operandA.int32);
    mu_assert("Stack pointer not empty!", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_STORE_2_Int() {
    union TypesUnion operandA;
    operandA.int32 = 2048;
    union TypesUnion operandB;
    operandB.int32 = 1024;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        CONSTI,  operandA.bytes.low, 
                operandA.bytes.midLow, 
                operandA.bytes.midHigh, 
                operandA.bytes.high,
        STORE, 0,
        CONSTI,  operandB.bytes.low, 
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

    int storeda = vm->localScope->localsArray[0].value.int32;
    int storedb = vm->localScope->localsArray[1].value.int32;

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stored value does not match pushed value", storeda == operandA.int32);
    mu_assert("Stored value does not match pushed value", storedb == operandB.int32);
    mu_assert("Stack not empty", vm->sp == -1);

    vm_Free(vm);
    return 0;
}

char* test_STORE_2_Int_AddressOutOfBounds() {
    union TypesUnion operandA;
    operandA.int32 = 2048;
    union TypesUnion operandB;
    operandB.int32 = 1024;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        CONSTI, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh, operandB.bytes.high,
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

    int storeda = vm->localScope->localsArray[0].value.int32;

    mu_assert("Expected error state MEM_SEG_FAULT not set", vm->errorCode == MEM_SEG_FAULT);
    mu_assert("Stored value does not match pushed value", storeda == operandA.int32);
    mu_assert("Operand stack not in expected state", vm->sp == 0);

    vm_Free(vm);
    return 0;
}

char* test_LOAD_1_Int() {
    union TypesUnion operandA;
    operandA.int32 = -2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
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

    //int stored = vm->funcScope->locals[0].value.int32;
    int loaded = vm->opStack[0].value.int32; //Get value from top of stack.
    int stored = vm->localScope->localsArray[0].value.int32;

    //uint8_t error = vm->errorCode;
    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Value not stored correctly", stored == operandA.int32);
    mu_assert("Loaded value not on stack!", (vm->sp == 0));
    mu_assert("Loaded value does not match pushed value", loaded == operandA.int32);

    vm_Free(vm);
    return 0;
}

char* test_LOAD_3_Int() {
    vm_value operandA;
    operandA.int32 = 16000;
    vm_value operandB;
    operandB.int32 = 24000;
    vm_value operandC;
    operandC.int32 = 32000;


    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 3, //Function address low byte, high byte, args count, memory allocation
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        CONSTI, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh, operandB.bytes.high,
        STORE, 1,
        CONSTI, operandC.bytes.low, operandC.bytes.midLow, operandC.bytes.midHigh, operandC.bytes.high,
        STORE, 2,
        LOAD, 2,
        LOAD, 1,
        LOAD, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              3, // number type globals to be reserved
                              33, //code size
                              20); //Stack size

    vm_Run(vm);

    //StackDump(vm);

    int loadedA = vm->opStack[2].value.int32; //Get value from top of stack.
    int loadedB = vm->opStack[1].value.int32; //Get value from top of stack.
    int loadedC = vm->opStack[0].value.int32; //Get value from top of stack.

    int storedA = vm->localScope->localsArray[0].value.int32;
    int storedB = vm->localScope->localsArray[1].value.int32;
    int storedC = vm->localScope->localsArray[2].value.int32;

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Loaded values not on stack!", (vm->sp == 2));
    mu_assert("Value not stored correctly", (storedA == operandA.int32) & (storedB == operandB.int32) & (storedC == operandC.int32));
    mu_assert("Loaded values do not match pushed values", (loadedA == operandA.int32) & (loadedB == operandB.int32) & (loadedC == operandC.int32));


    vm_Free(vm);
    return 0;
}

char* test_LOAD_Int_AddressOutOfBounds() {
    vm_value operandA;
    operandA.int32 = -33;
    vm_value operandB;
    operandB.int32 = -99;
    vm_value operandC;
    operandC.int32 = -1000;


    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 3, //Function address low byte, high byte, args count, memory allocation
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        CONSTI, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh, operandB.bytes.high,
        STORE, 1,
        CONSTI, operandC.bytes.low, operandC.bytes.midLow, operandC.bytes.midHigh, operandC.bytes.high,
        STORE, 2,
        LOAD, 2,
        LOAD, 1,
        LOAD, 3,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              33, //code size
                              20); //Stack size

    vm_Run(vm);

    //StackDump(vm);

    int loadedB = vm->opStack[1].value.int32; //Get value from top of stack.
    int loadedC = vm->opStack[0].value.int32; //Get value from top of stack.

    int storedA = vm->localScope->localsArray[0].value.int32;
    int storedB = vm->localScope->localsArray[1].value.int32;
    int storedC = vm->localScope->localsArray[2].value.int32;

    mu_assert("Expected error state MEM_SEG_FAULT not set", vm->errorCode == MEM_SEG_FAULT);
    mu_assert("Loaded values not on stack!", (vm->sp == 1));
    mu_assert("Values not stored correctly", (storedA == operandA.int32) & (storedB == operandB.int32) & (storedC == operandC.int32));
    mu_assert("Loaded values do not match pushed values", (loadedB == operandB.int32) & (loadedC == operandC.int32));

    vm_Free(vm);
    return 0;
}

char* test_LOAD_Int_LoadBeforeStore() {
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

char* test_STORE_Int_StringAlreadyStoredAtAddress() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0,
        CONSTI8, 128,
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

//TOTO: Add test for STORE/LOAD when no local function scope exists or create main funciton scope.

char* test_STORE_LOAD_Int_HighestAddressableIndex() {
    union TypesUnion operandA;
    operandA.int32 = 2048;

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

    //int stored = vm->funcScope->locals[0].value.int32;
    int loaded = vm->opStack[0].value.int32; //Get value from top of stack.
    int stored = vm->localScope->localsArray[254].value.int32;

    mu_assert("Not Expected error state", vm->errorCode == OK);
    mu_assert("Value not stored correctly", stored == operandA.int32);
    mu_assert("Loaded value not on stack!", (vm->sp == 0));
    mu_assert("Loaded value does not match pushed value", loaded == operandA.int32);

    vm_Free(vm);
    return 0;
}

char* test_GSTORE_1_Int() {
    union TypesUnion operandA;
    operandA.int32 = 24000;

    uint8_t program[] = {
        // entrypoint - main function        
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        GSTORE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // number type globals to be reserved
                              8, //code size
                              20); 
    
    vm_Run(vm);

    //StackDump(vm);

    int stored = vm->globalsArray[0].value.int32;

    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Stored value does not match pushed value", stored == operandA.int32);
    mu_assert("Stack pointer not empty!", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_GLOAD_1_Int() {
    union TypesUnion operandA;
    operandA.int32 = -2048;

    uint8_t program[] = {
        // entrypoint - main function        
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        GSTORE, 0, //TOTO: Add test for GSTORE/GLOAD when no globals declared.
        GLOAD, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // number type globals to be reserved
                              10, //code size
                              20); //Stack size

    vm_Run(vm);
    //StackDump(vm);

    //int stored = vm->funcScope->locals[0].value.int32;
    int loaded = vm->opStack[0].value.int32; //Get value from top of stack.
    int stored = vm->globalsArray[0].value.int32;

    //uint8_t error = vm->errorCode;
    mu_assert("Expected error state OK", vm->errorCode == OK);
    mu_assert("Value not stored correctly", stored == operandA.int32);
    mu_assert("Loaded value not on stack!", (vm->sp == 0));
    mu_assert("Loaded value does not match pushed value", loaded == operandA.int32);

    vm_Free(vm);
    return 0;
}

char* test_GLOAD_NoGlobalsDeclared() {
   
    uint8_t program[] = {
        // entrypoint - main function       
        GLOAD, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              3, //code size
                              20); //Stack size

    vm_Run(vm);
    
    //uint8_t error = vm->errorCode;
    mu_assert("Expected error state OK", vm->errorCode == MEM_SEG_FAULT);

    vm_Free(vm);
    return 0;
}

char* test_GSTORE_NoGlobalsDeclared() {
 
    uint8_t program[] = {
        // entrypoint - main function       
        GSTORE, 0, //TOTO: Add test for GSTORE/GLOAD when no globals declared.
        GLOAD, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              10, //code size
                              20); //Stack size

    vm_Run(vm);
    mu_assert("Expected error state OK", vm->errorCode == MEM_SEG_FAULT);

    vm_Free(vm);
    return 0;
}

char* test_STORE_NoLocalsDeclared() {
    union TypesUnion operandA;
    operandA.int32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 0, //Function address low byte, high byte, args count, memory allocation
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
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
    mu_assert("Expected error state OK", vm->errorCode == MEM_SEG_FAULT);

    vm_Free(vm);
    return 0;
}

char* test_LOAD_NoLocalsDeclared() {

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 0, //Function address low byte, high byte, args count, memory allocation
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
    mu_assert("Expected error state OK", vm->errorCode == MEM_SEG_FAULT);

    vm_Free(vm);
    return 0;
}

char* test_STORE_NoFunctionScope() {
    union TypesUnion operandA;
    operandA.int32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function        
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              8, //code size
                              20); 
    
    vm_Run(vm);

    mu_assert("Expected error state OK", vm->errorCode == MEM_SEG_FAULT);

    vm_Free(vm);
    return 0;
}

char* test_LOAD_NoFunctionScope() {
    
    uint8_t program[] = {
        // entrypoint - main function       
        LOAD, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              3, //code size
                              20); 
    
    vm_Run(vm);

    mu_assert("Expected error state OK", vm->errorCode == MEM_SEG_FAULT);

    vm_Free(vm);
    return 0;
}

