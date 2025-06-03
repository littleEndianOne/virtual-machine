#ifndef TESTS_JUMP_H_INCLUDED
#define TESTS_JUMP_H_INCLUDED

char* test_JMP();
char* test_JMPT_TRUE();
char* test_JMPT_FALSE();
char* test_JMPF_FALSE();
char* test_JMPF_TRUE();
char* test_JMP_JumpToAddressOutOfBounds();
char* test_JMPT_TRUE_JumpToAddressOutOfBounds();
char* test_JMPF_FALSE_JumpToAddressOutOfBounds();
char* test_JMP_JumpTo16Address();
char* test_JMPT_FALSE_Float();
char* test_JMPT_TRUE_Float();

void RunSet_JUMP();

#endif // TESTS_JUMP_H_INCLUDED
