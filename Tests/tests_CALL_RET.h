#ifndef TESTS_CALL_H_INCLUDED
#define TESTS_CALL_H_INCLUDED

char* test_CALL_NoArgs();
char* test_CALL_InvalidArgIndex();
char* test_CALL_NoArgPushedBeforeLoad();
char* test_CALL_NumArg();
char* test_CALL_2_NumArg();
char* test_CALL_NoReturnValue();
char* test_CALL_LocalAllocation();
char* test_CALL_3_NumArg();
char* test_CALL_2_Func();
char* test_CALL_3_NumArgReturn();
char* test_CALL_MultipleCalls();
char* test_RET_NoFunc();
char* test_CALL_NestedCalls();

void RunSet_CALL();

#endif // TESTS_CALL_H_INCLUDED
