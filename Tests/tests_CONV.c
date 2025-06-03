#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "tests_CONV.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_CONV() {
    //printf("\n\n\nTest CONV Operator: \n\n");

    mu_run_test(test_CONV_SmallValue);
    mu_run_test(test_CONV_Truncated);
    mu_run_test(test_CONV_MaxPrecision);
    mu_run_test(test_CONV_ZeroPrecision);
    mu_run_test(test_CONV_LargeValue);
    mu_run_test(test_CONV_LargeFractal);
    mu_run_test(test_CONV_NegativeValue);
    mu_run_test(test_CONV_WrongType_Source);
    mu_run_test(test_CONV_WrongType_Destination);
    mu_run_test(test_CONV_ZeroLengthString);
    mu_run_test(test_CONV_Integer);
}

char* test_CONV_SmallValue() {
    union TypesUnion operandA;
    operandA.float32 = 2048.24;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 14, 0, //Create string allocation with length 14 at address 0.
        LOAD, 0,
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        TOSTR, 2, //Convert number to string with 2 dp to string at 0.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              18, 
                              20); //code size

    vm_Run(vm);

    char exptectedString[] = "2048.24";
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

char* test_CONV_Truncated() {
    union TypesUnion operandA;
    operandA.float32 = 2048.24;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 5, 0, //Create string allocation with length 14 at address 0.
        LOAD, 0,
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        TOSTR, 2, //Convert number to string with 2 dp to string at 0.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              18, 
                              20); //code size

    vm_Run(vm);

    char exptectedString[] = "2048";
    //printf("String generated: %s \n", vm->localScope->localsArray[0].value.strRef.charPtr);
    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->localScope->localsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->localScope->localsArray[0].value.strRef.allocLength == 5);

    vm_Free(vm);
    return 0;
}

char* test_CONV_MaxPrecision() {
    union TypesUnion operandA;
    operandA.float32 = 32.00;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 16, 0, //Create string allocation with length 14 at address 0.
        LOAD, 0,
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        TOSTR, 12, //Convert number to string with 2 dp to string at 0.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              18, 
                              20); //code size
    vm_Run(vm);

    char exptectedString[] = {'3', '2', '.', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0'};
    //printf("String pushed: %s \n", vm->opStack->value.strPtr);

    //printf("String generated: %s \n", vm->localScope->localsArray[0].value.strRef.charPtr);
    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->localScope->localsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->localScope->localsArray[0].value.strRef.allocLength == 16);

    vm_Free(vm);
    return 0;
}

char* test_CONV_ZeroPrecision() {
    union TypesUnion operandA;
    operandA.float32 = 32.001235643;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 16, 0, //Create string allocation with length 14 at address 0.
        LOAD, 0,
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        TOSTR, 0, //Convert number to string with 2 dp to string at 0.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              18, 
                              20); //code size
    vm_Run(vm);

    char exptectedString[] = {'3', '2', '\0'};
    //printf("String pushed: %s \n", vm->opStack->value.strPtr);

    //printf("String generated: %s \n", vm->localScope->localsArray[0].value.strRef.charPtr);
    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->localScope->localsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->localScope->localsArray[0].value.strRef.allocLength == 16);

    vm_Free(vm);
    return 0;
}

char* test_CONV_LargeValue() {
    union TypesUnion operandA;
    operandA.float32 = 32000124;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 16, 0, //Create string allocation with length 14 at address 0.
        LOAD, 0,
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        TOSTR, 0, //Convert number to string with 2 dp to string at 0.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              18, 
                              20); //code size
    vm_Run(vm);

    char exptectedString[] = {'3', '2', '0', '0', '0', '1', '2', '4', '\0'};

    //printf("String generated: %s \n", vm->localScope->localsArray[0].value.strRef.charPtr);
    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->localScope->localsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->localScope->localsArray[0].value.strRef.allocLength == 16);

    vm_Free(vm);
    return 0;
}

char* test_CONV_LargeFractal() {
    union TypesUnion operandA;
    operandA.float32 = 0.0123456;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 16, 0, //Create string allocation with length 14 at address 0.
        LOAD, 0,
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        TOSTR, 7, //Convert number to string with 2dp to string at 0.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              18, //code size
                              20); 

    vm_Run(vm);

    char exptectedString[] = {'0', '.', '0', '1', '2', '3', '4', '5', '6', '\0'};
    //printf("String pushed: %s \n", vm->opStack->value.strPtr);

    //printf("String generated: %s \n", vm->localScope->localsArray[0].value.strRef.charPtr);
    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->localScope->localsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->localScope->localsArray[0].value.strRef.allocLength == 16);

    vm_Free(vm);
    return 0;
}

char* test_CONV_NegativeValue() {
    union TypesUnion operandA;
    operandA.float32 = -512.128;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 16, 0, //Create string allocation with length 14 at address 0.
        LOAD, 0,
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        TOSTR, 3, //Convert number to string with 2 dp to string at 0.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              18, 
                              20); //code size

    vm_Run(vm);

    char exptectedString[] = {'-', '5', '1', '2', '.', '1', '2', '8', '\0'};
    //printf("String pushed: %s \n", vm->opStack->value.strPtr);

    //printf("String generated: %s \n", vm->localScope->localsArray[0].value.strRef.charPtr);
    mu_assert("Error state not OK", vm->errorCode == OK);
    //char ptr on stack
    mu_assert("Stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("String stored does not match expected.",
              !(strcmp(exptectedString,
                       vm->localScope->localsArray[0].value.strRef.charPtr)));
    mu_assert("String allocation length not as expected", vm->localScope->localsArray[0].value.strRef.allocLength == 16);

    vm_Free(vm);
    return 0;
}

char* test_CONV_WrongType_Source() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 4, 0,
        LOAD, 0,
        STRLIT, 6, 's', 't', 'r', 'i', 'n', 'g', '\0',
        TOSTR, 3,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function                  
                              0,
                              21, 
                              20);
    vm_Run(vm);

    //StackDump(vm);

    mu_assert("Op stack not empty!", (vm->sp == -1));
    //string stored in allocated memory
    mu_assert("Error state not as expected", vm->errorCode == TYPE_ERROR);

    vm_Free(vm);
    return 0;
}

char* test_CONV_WrongType_Destination() {
    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 4, 0,
        CONSTF8, 8,
        CONSTF8, 32,
        TOSTR, 3,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function                  
                              0,
                              22, 
                              20);
    vm_Run(vm);

    //StackDump(vm);

    mu_assert("Error state not as expected", vm->errorCode == TYPE_ERROR);
    mu_assert("Op stack not empty!", (vm->sp == -1));
    //string stored in allocated memory


    vm_Free(vm);
    return 0;
}

char* test_CONV_ZeroLengthString() {
    union TypesUnion operandA;
    operandA.float32 = 2048.24;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 1, 0, //Create string allocation with length 1 at address 0.
        LOAD, 0,
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        TOSTR, 1, //Convert number to string with 2 dp to string at 0.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              18, 
                              20); //code size

    vm_Run(vm);

    char exptectedString[] = "";
    //printf("String generated: %s \n", vm->localScope->localsArray[0].value.strRef.charPtr);
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

char* test_CONV_Integer() {
    union TypesUnion operandA;
    operandA.int32 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CALL, 5, 0, 0, 1, //Function address low byte, high byte, args count, memory allocation
        DSTR, 14, 0, //Create string allocation with length 14 at address 0.
        LOAD, 0,
        CONSTI, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh, operandA.bytes.high,
        TOSTR, //Convert number to string with 2 dp to string at 0.
        HALT //stop program before return to preserve memory.
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // number type globals to be reserved
                              17, //code size
                              20); //Stack size

    vm_Run(vm);

    char exptectedString[] = "2048";
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
