#ifndef TESTS_MATH_H_INCLUDED
#define TESTS_MATH_H_INCLUDED

char* test_ADDF_PositiveNumbers();
char* test_ADDF_NegativeNumbers();
char* test_SUBF_NegativeResult();
char* test_MULF_PositiveNumbers();
char* test_MULF_NegativeNumbers();
char* test_MUL_NegativeAndPositiveNumber();

char* test_DIVF();
char* test_DIVF_Div0();

char* test_BinaryOperator_MissingOperand();

void RunSet_MATH_F();

#endif // TESTS_MATH_F_H_INCLUDED
