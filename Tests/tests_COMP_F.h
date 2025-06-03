#ifndef TESTS_COMP_H_INCLUDED
#define TESTS_COMP_H_INCLUDED

char* test_EQF_OperandsNotEqual();
char* test_EQF_OperandsEqual();
char* test_NEQF_OperandsNotEqual();
char* test_NEQF_Operands_Equal();
char* test_LTF_ALessThanB();
char* test_LTF_AGreaterThanB();
char* test_LTF_OperandsEqual();
char* test_GTF_ALessThanB();
char* test_GTF_AGreaterThanB();
char* test_GTF_OperandsEqual();

void RunSet_COMP_F();

#endif // TESTS_COMP_F_H_INCLUDED
