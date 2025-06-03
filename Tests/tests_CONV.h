#ifndef TESTS_CONV_H_INCLUDED
#define TESTS_CONV_H_INCLUDED

char* test_CONV_SmallValue();
char* test_CONV_Truncated();
char* test_CONV_MaxPrecision();
char* test_CONV_ZeroPrecision();
char* test_CONV_LargeValue();
char* test_CONV_LargeFractal();
char* test_CONV_NegativeValue();
char* test_CONV_WrongType_Source();
char* test_CONV_WrongType_Destination();
char* test_CONV_ZeroLengthString();
char* test_CONV_Integer();
void RunSet_CONV();

#endif // TESTS_CONV_H_INCLUDED
