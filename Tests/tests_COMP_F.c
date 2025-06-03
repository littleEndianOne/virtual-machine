#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_COMP_F.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_COMP_F() {
    printf("\n\n\nTest Float Comparison Operators: \n\n");
    mu_run_test(test_EQF_OperandsNotEqual);
    mu_run_test(test_EQF_OperandsEqual);
    mu_run_test(test_NEQF_OperandsNotEqual);
    mu_run_test(test_NEQF_Operands_Equal);
    mu_run_test(test_LTF_ALessThanB);
    mu_run_test(test_LTF_OperandsEqual);
    mu_run_test(test_LTF_AGreaterThanB);
    mu_run_test(test_GTF_ALessThanB);
    mu_run_test(test_GTF_OperandsEqual);
    mu_run_test(test_GTF_AGreaterThanB);
}

char* test_EQF_OperandsNotEqual() {
    return TestFloatBinaryOperator(EQF, 1000.0001, -1000.0001, FALSE, FALSE);
}

char* test_EQF_OperandsEqual() {
    return TestFloatBinaryOperator(EQF, 1000.0001, 1000.0001, TRUE, FALSE);
}

char* test_NEQF_OperandsNotEqual() {
    return TestFloatBinaryOperator(NEQF, 1000.0001, -1000.0001, TRUE, FALSE);
}

char* test_NEQF_Operands_Equal() {
    return TestFloatBinaryOperator(NEQF, 1000.0001, 1000.0001, FALSE, FALSE);
}

char* test_LTF_OperandsEqual() {
    return TestFloatBinaryOperator(GTF, 1000.0001, 1000.0001, FALSE, FALSE);
}

char* test_LTF_ALessThanB() {
    return TestFloatBinaryOperator(LTF, -1000.0001, 1000.0001, TRUE, FALSE);
}

char* test_LTF_AGreaterThanB() {
    return TestFloatBinaryOperator(LTF, 1000.0001, -1000.0001, FALSE, FALSE);
}

char* test_GTF_AGreaterThanB() {
    return TestFloatBinaryOperator(GTF, 1000.0001, -1000.0001, TRUE, FALSE);
}

char* test_GTF_OperandsEqual() {
    return TestFloatBinaryOperator(GTF, -1000.0001, -1000.0001, FALSE, FALSE);
}

char* test_GTF_ALessThanB() {
    return TestFloatBinaryOperator(GTF, -1000.0001, 1000.0001, FALSE, FALSE);
}


