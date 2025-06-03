#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "tests_Storage_Str.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_STORE_LOAD_Str() {
    printf("\n\n\nTest Storage Operators for StringRef & StringLit types: \n\n");

    mu_run_test(test_STORE_1_StrLit);
    mu_run_test(test_STORE_1_StrLit_EmptyString);
    mu_run_test(test_STORE_1_StrLit_AddressOutOfBounds);
    mu_run_test(test_STORE_1_StrLit_StringTooLongForExistingAllocation);
    mu_run_test(test_STORE_1_StrLit_LargeExistingAllocation);
    mu_run_test(test_LOAD_1_StrRef);
    mu_run_test(test_STORE_1_StrRef);
    mu_run_test(test_STORE_1_StrLit_NumberTypeAlreadyAtAddress);

    mu_run_test(test_STORE_1_StrRef_AddressOutOfBounds);
    mu_run_test(test_STORE_1_StrRef_StringTooLongForExistingAllocation);
    mu_run_test(test_STORE_1_StrRef_LargeExistingAllocation);

    mu_run_test(test_STORE_1_StrRef_NumberTypeAlreadyAtAddress);
    mu_run_test(test_STORE_1_StrRef_EmptyString);
    
    /*The code used to implement the global storage operators is the same
     * as the local operators so these tests are just to test to ensure the 
     * plumbing is in place. 
     */
    mu_run_test(test_GSTORE_1_StrLit);
    mu_run_test(test_GLOAD_1_StrRef);
    mu_run_test(test_GSTORE_1_StrRef);

}

char* test_STORE_1_StrLit() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 0, '\0',
        STORE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              11, //code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;
    char* expected = "";

    mu_assert("Expected error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 1);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrLit_EmptyString() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0,
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              17, // code size
                              20); // stack size

    vm_Run(vm);

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;
    char* expected = "hello";

    mu_assert("Expected error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 6);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrLit_AddressOutOfBounds() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 2, 0, //Attempt to store starting at last adressable byte.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              17, //code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm);

    mu_assert("error state not as expected", vm->errorCode == MEM_SEG_FAULT);
    mu_assert("Run state not as expected", vm->runState == FAULT);
    mu_assert("String lit not on stack", (vm->sp == 0));


    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrLit_StringTooLongForExistingAllocation() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0, //Attempt to store starting at last addressable byte.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              16, //code size
                              20); //stack size

    vm_StartDebug(vm);
    vm_DebugRunFor(vm, 1); //Run call

    //Setup existing string reference of specific size.
    uint8_t existingSize = 3;
    vm->localScope->localsArray[0].type = STRING_REF;
    vm->localScope->localsArray[0].value.strRef.charPtr = malloc(sizeof (char)*existingSize);
    vm->localScope->localsArray[0].value.strRef.charPtr[2] = '\0';
    vm->localScope->localsArray[0].value.strRef.allocLength = existingSize;

    vm_DebugRun(vm); //Continue program execution.

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;
    char* expected = "he";
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 3);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrLit_LargeExistingAllocation() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0, //Attempt to store starting at last adressable byte.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, //globals to be reserved
                              16, //code size
                              20); //stack size

    vm_StartDebug(vm);
    vm_DebugRunFor(vm, 1); //Run call

    //Setup existing string reference of specific size.
    uint8_t existingSize = 30;
    vm->localScope->localsArray[0].type = STRING_REF;
    vm->localScope->localsArray[0].value.strRef.charPtr = malloc(sizeof (char)*existingSize);
    vm->localScope->localsArray[0].value.strRef.charPtr[2] = '\0';
    vm->localScope->localsArray[0].value.strRef.allocLength = existingSize;

    vm_DebugRun(vm); //Continue program execution.

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;
    char* expected = "hello";
    mu_assert("Expected error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 30);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_LOAD_1_StrRef() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0,
        LOAD, 0,
        HALT //stop program before return to preserve local memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              18, //code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    char* expected = "hello";

    mu_assert("Not Expected error state", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));

    mu_assert("Loaded value not on stack", (vm->sp == 0));
    mu_assert("Loaded reference not correct type", vm->opStack[0].type == STRING_REF); //check element type.
    mu_assert("Stored value does not match pushed value", !strcmp(expected, vm->opStack[0].value.strRef.charPtr));

    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrRef() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0,
        LOAD, 0,
        STORE, 1,
        HALT //stop program before return to preserve local memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              22, //code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[1].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[1].type;
    uint16_t size = vm->localScope->localsArray[1].value.strRef.allocLength;
    char* expected = "hello";

    mu_assert("Expected error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrect!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 6);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrLit_NumberTypeAlreadyAtAddress() {
    union TypesUnion operandA;
    operandA.float32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        STORE, 0,
        STRLIT, 2, 'h', 'i', '\0',
        STORE, 0,
        HALT //stop program before return to preserve memory allocation.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, //  globals to be reserved
                              21, //code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm);

    float stored = vm->localScope->localsArray[0].value.float32;

    mu_assert("Not expected error state", vm->errorCode == TYPE_ERROR);
    mu_assert("Stored value does not match pushed value", stored == operandA.float32);
    mu_assert("Stack not empty!", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrRef_AddressOutOfBounds() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0,
        LOAD, 0,
        STORE, 2,
        HALT //stop program before return to preserve local memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, //globals to be reserved
                              22, //code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm);   

    mu_assert("Expected error state not as expected", vm->errorCode == MEM_SEG_FAULT);
    mu_assert("Run state not as expected", vm->runState == FAULT);
    mu_assert("Stack not as expected", (vm->sp == 0));

    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrRef_StringTooLongForExistingAllocation() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0, //Attempt to store starting at last addressable byte.
        LOAD, 0,
        STORE, 1, //Store loaded string ref into memory already created externally.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, //globals to be reserved
                              20, //code size
                              20); //stack size

    vm_StartDebug(vm);
    vm_DebugRunFor(vm, 4); //Run to (STORE, 1) when String reference from index 0 is copied into index 1.

    //Setup existing string reference of specific size.
    uint8_t existingSize = 3;
    vm->localScope->localsArray[1].type = STRING_REF;
    vm->localScope->localsArray[1].value.strRef.charPtr = malloc(sizeof (char)*existingSize);
    vm->localScope->localsArray[1].value.strRef.charPtr[2] = '\0';
    vm->localScope->localsArray[1].value.strRef.allocLength = existingSize;

    vm_DebugRun(vm); //Continue program execution.

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[1].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[1].type;
    uint16_t size = vm->localScope->localsArray[1].value.strRef.allocLength;
    char* expected = "he";
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == existingSize);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrRef_LargeExistingAllocation() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0, //Attempt to store starting at last addressable byte.
        LOAD, 0,
        STORE, 1, //Store loaded string ref into memory already created externally.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              20, //code size
                              20); //stack size

    vm_StartDebug(vm);
    vm_DebugRunFor(vm, 4); //Run to (STORE, 1) when String reference from index 0 is copied into index 1.

    //Setup existing string reference of specific size.
    uint8_t existingSize = 14;
    vm->localScope->localsArray[1].type = STRING_REF;
    vm->localScope->localsArray[1].value.strRef.charPtr = malloc(sizeof (char)*existingSize);
    vm->localScope->localsArray[1].value.strRef.charPtr[2] = '\0';
    vm->localScope->localsArray[1].value.strRef.allocLength = existingSize;

    vm_DebugRun(vm); //Continue program execution.

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[1].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[1].type;
    uint16_t size = vm->localScope->localsArray[1].value.strRef.allocLength;
    char* expected = "hello";
    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == existingSize);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_STORE_1_StrRef_NumberTypeAlreadyAtAddress() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 0, '\0',
        STORE, 0,
        CONSTF8, 64,
        STORE, 1,
        LOAD, 0,
        STORE, 1,
        HALT //stop program before return to preserve local memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              19, //code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm); 

    mu_assert("Expected error state not as expected", vm->errorCode == TYPE_ERROR);
    mu_assert("Run state not as expected", vm->runState == FAULT);
    mu_assert("Stack not as expected", (vm->sp == -1));

    vm_Free(vm);

    return 0;
}

char* test_STORE_1_StrRef_EmptyString() {
    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 0, '\0',
        STORE, 0,
        LOAD, 0,
        STORE, 1,
        HALT //stop program before return to preserve local memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // globals to be reserved
                              15, //code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm);

    char* storedStr = vm->localScope->localsArray[1].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[1].type;
    uint16_t size = vm->localScope->localsArray[1].value.strRef.allocLength;
    char* expected = "";

    mu_assert("Expected error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrect!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 1);
    mu_assert("Stack not empty", (vm->sp == -1));

    
    vm_Free(vm);
    return 0;
}

char* test_GSTORE_1_StrLit() {
    uint8_t program[] = {
        // entrypoint - main function        
        STRLIT, 2, 'O', 'K', '\0',
        GSTORE, 0,
        HALT 
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // globals to be reserved
                              8, //code size
                              20); //stack size

    vm_Run(vm);

    char* storedStr = vm->globalsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->globalsArray[0].type;
    uint16_t size = vm->globalsArray[0].value.strRef.allocLength;
    char* expected = "OK";

    mu_assert("Expected error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 3);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_GLOAD_1_StrRef() {
    uint8_t program[] = {
        // entrypoint - main function       
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        GSTORE, 0,
        GLOAD, 0,
        HALT //stop program before return to preserve local memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, // globals to be reserved
                              13, //code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm);

    char* storedStr = vm->globalsArray[0].value.strRef.charPtr;
    char* expected = "hello";

    mu_assert("Not Expected error state", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));

    mu_assert("Loaded value not on stack", (vm->sp == 0));
    mu_assert("Loaded reference not correct type", vm->opStack[0].type == STRING_REF); //check element type.
    mu_assert("Stored value does not match pushed value", !strcmp(expected, vm->opStack[0].value.strRef.charPtr));

    vm_Free(vm);
    return 0;
}

char* test_GSTORE_1_StrRef() {
    uint8_t program[] = {
        // entrypoint - main function
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        GSTORE, 0,
        GLOAD, 0,
        GSTORE, 1,
        HALT //stop program before return to preserve local memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              2, // globals to be reserved
                              15, // code size
                              20); //stack size

    vm_Run(vm);

    //StackDump(vm);

    char* storedStr = vm->globalsArray[1].value.strRef.charPtr;
    vm_type typeSet = vm->globalsArray[1].type;
    uint16_t size = vm->globalsArray[1].value.strRef.allocLength;
    char* expected = "hello";

    mu_assert("Expected error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrect!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 6);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}


