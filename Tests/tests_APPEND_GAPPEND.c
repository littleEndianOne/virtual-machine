#include <stdint-gcc.h>
#include <string.h>
#include "tests_APPEND_GAPPEND.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_APPEND() {
    printf("\n\n\nTest APPEND Operator: \n\n");

    mu_run_test(test_APPEND_Local_StrLit);
    mu_run_test(test_APPEND_Local_StrRef);
    mu_run_test(test_APPEND_Local_Float);
    mu_run_test(test_APPEND_Local_Int);

    mu_run_test(test_APPEND_EmptyStrRef);
    mu_run_test(test_APPEND_ToEmptyStr);

    mu_run_test(test_APPEND_CauseOverflow);
    mu_run_test(test_APPEND_AppendToFull);

    mu_run_test(test_APPEND_BuildString);
    mu_run_test(test_APPEND_ToMinAllocLength);
 
    /*The code used to implement the global storage operators is the same
     * as the local operators so these tests are just to test to ensure the 
     * plumbing is in place for the global operator. 
     */
    mu_run_test(test_APPEND_Global_StrLit);
}

char* test_APPEND_Local_StrLit() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 14, 0,
        STRLIT, 6, 'h', 'e', 'l', 'l', 'o', ' ', '\0',
        STORE, 0,
        LOAD, 0,
        STRLIT, 6, 'w', 'o', 'r', 'l', 'd', '!', '\0',
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              32, 20);
    vm_Run(vm);

    //    StartDebug(vm);    
    //    DebugRunFor(vm, 6);
    //    StackDump(vm);
    //    DebugRun(vm);


    char expected[] = {'h', 'e', 'l', 'l', 'o', ' ',
        'w', 'o', 'r', 'l', 'd', '!', '\0'};


    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    //printf("Actual: %s \n", storedStr);

    mu_assert("Expected error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 14);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_Local_StrRef() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        DSTR, 14, 0,
        STRLIT, 6, 'h', 'e', 'l', 'l', 'o', ' ', '\0',
        STORE, 0,
        STRLIT, 6, 'w', 'o', 'r', 'l', 'd', '!', '\0',
        STORE, 1,
        LOAD, 0,
        LOAD, 1,
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              36, 20);
    vm_Run(vm);


    char expected[] = {'h', 'e', 'l', 'l', 'o', ' ',
        'w', 'o', 'r', 'l', 'd', '!', '\0'};


    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    //printf("Actual: %s \n", storedStr);

    mu_assert("Expected error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 14);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_Local_Float() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        DSTR, 14, 0,
        STRLIT, 6, 'h', 'e', 'l', 'l', 'o', ' ', '\0',
        STORE, 0,
        LOAD, 0,
        CONSTF8, 127,
        APPND, 2,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              26, 20);
    vm_Run(vm);

    char expected[] = {'h', 'e', 'l', 'l', 'o', ' ',
        '1', '2', '7', '.', '0', '0', '\0'};


    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    //printf("Actual: %s \n", storedStr);

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 14);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_Local_Int() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        DSTR, 14, 0,
        STRLIT, 6, 'h', 'e', 'l', 'l', 'o', ' ', '\0',
        STORE, 0,
        LOAD, 0,
        CONSTI8, 127,
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              25, 
                              20);
    vm_Run(vm);

    char expected[] = {'h', 'e', 'l', 'l', 'o', ' ',
        '1', '2', '7', '\0'};


    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    //printf("Actual: %s \n", storedStr);

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 14);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_Global_StrLit() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 0, //Function address low byte, high byte, args count, memory allocation
        GDSTR, 14, 0,
        STRLIT, 6, 'h', 'e', 'l', 'l', 'o', ' ', '\0',
        GSTORE, 0,
        GLOAD, 0,
        STRLIT, 6, 'w', 'o', 'r', 'l', 'd', '!', '\0',
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              1, //Globals to reserve
                              32, 20);
    vm_Run(vm);

    char expected[] = {'h', 'e', 'l', 'l', 'o', ' ',
        'w', 'o', 'r', 'l', 'd', '!', '\0'};


    char* storedStr = vm->globalsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->globalsArray[0].type;
    uint16_t size = vm->globalsArray[0].value.strRef.allocLength;

    //printf("Actual: %s \n", storedStr);

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 14);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_EmptyStrRef() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        DSTR, 14, 0,
        DSTR, 1, 1,
        STRLIT, 6, 'h', 'e', 'l', 'l', 'o', ' ', '\0',
        STORE, 0,
        LOAD, 0,
        LOAD, 1,
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              28, 20);
    vm_Run(vm);

    char expected[] = {'h', 'e', 'l', 'l', 'o', ' ', '\0'};


    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    //printf("Actual: %s \n", storedStr);

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 14);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_ToEmptyStr() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        DSTR, 14, 0,
        LOAD, 0,
        STRLIT, 6, 'h', 'e', 'l', 'l', 'o', ' ', '\0',
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              21, 20);
    vm_Run(vm);

    char expected[] = {'h', 'e', 'l', 'l', 'o', ' ', '\0'};


    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    //printf("Actual: %s \n", storedStr);

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 14);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_BuildString() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        DSTR, 17, 0,
        STRLIT, 6, 't', 'e', 'm', 'p', ':', ' ', '\0',
        STORE, 0,
        LOAD, 0,
        CONSTF8, 122,
        APPND, 2,
        LOAD, 0,
        STRLIT, 4, ' ', 'd', 'e', 'g', '\0',
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              36, 20);
    vm_Run(vm);

    //    StartDebug(vm);    
    //    DebugRunFor(vm, 9);
    //    StackDump(vm);
    //    DebugRun(vm);

    char expected[] = "temp: 122.00 deg";


    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    //printf("Actual: %s \n", storedStr);

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 17);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_ToMinAllocLength() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        DSTR, 1, 0,
        LOAD, 0,
        STRLIT, 6, 'h', 'e', 'l', 'l', 'o', ' ', '\0',
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              21, 20);
    vm_Run(vm);

    char expected[] = {'\0'};

    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 1);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_CauseOverflow() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        DSTR, 4, 0,
        LOAD, 0,
        STRLIT, 6, 'h', 'e', 'l', 'l', 'o', ' ', '\0',
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              21, 20);
    vm_Run(vm);

    char expected[] = "hel";

    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 4);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}

char* test_APPEND_AppendToFull() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 2, //Function address low byte, high byte, args count, memory allocation
        STRLIT, 5, 'h', 'e', 'l', 'l', 'o', '\0',
        STORE, 0,
        LOAD, 0,
        STRLIT, 1, '!', '\0',
        APPND,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              24, 20);
    vm_Run(vm);

    char expected[] = "hello";

    char* storedStr = vm->localScope->localsArray[0].value.strRef.charPtr;
    vm_type typeSet = vm->localScope->localsArray[0].type;
    uint16_t size = vm->localScope->localsArray[0].value.strRef.allocLength;

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Run state not as expected", vm->runState == HALTED);
    mu_assert("Stored value does not match pushed value", !strcmp(expected, storedStr));
    mu_assert("Stored type set incorrectly!", typeSet == STRING_REF);
    mu_assert("String allocation size incorrect", size == 6);
    mu_assert("Stack not empty", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}
