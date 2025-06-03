#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_MATH_F.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_MATH_F() {
    printf("\n\n\nTest Floating Point Math Operators: \n\n");
    mu_run_test(test_ADDF_PositiveNumbers);
    mu_run_test(test_ADDF_NegativeNumbers);
    mu_run_test(test_SUBF_NegativeResult);
    mu_run_test(test_MULF_PositiveNumbers);
    mu_run_test(test_MULF_NegativeNumbers);
    mu_run_test(test_DIVF);
    mu_run_test(test_DIVF_Div0);   
}

char* test_ADDF_PositiveNumbers() {
    return TestFloatBinaryOperator(ADDF, 2048.32, 2048.32, 4096.64, FALSE);
}

char* test_ADDF_NegativeNumbers() {
    return TestFloatBinaryOperator(ADDF, -2048.32, -2048.32, -4096.64, FALSE);
}

char* test_SUBF_NegativeResult() {
    return TestFloatBinaryOperator(SUBF, 1024.64f, 4096.08, -3071.44, FALSE);
}

char* test_DIVF() {
    return TestFloatBinaryOperator(DIVF, 1024, 10, 102.4, FALSE);
}

char* test_MULF_PositiveNumbers() {
    return TestFloatBinaryOperator(MULF, 3276.7, 10, 32767, FALSE);
}

char* test_MULF_NegativeNumbers() {
    return TestFloatBinaryOperator(MULF, -3276.8, -10, 32768, FALSE);
}

char* test_DIVF_Div0() {
    union TypesUnion operandA;
    operandA.float32 = 100;
    union TypesUnion operandB;
    operandB.float32 = 0;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTF, operandA.bytes.low, operandA.bytes.midLow, operandA.bytes.midHigh,
        operandA.bytes.high,
        CONSTF, operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh,
        operandB.bytes.high,
        DIVF,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              12, 20);
    vm_Run(vm);

    mu_assert("Error state not DIV0", vm->errorCode == DIV0);
    mu_assert("Run state not FAULT", vm->runState == FAULT);
    mu_assert("Stack pointer not at -1!", (vm->sp == -1));

    vm_Free(vm);
    return 0;
}


