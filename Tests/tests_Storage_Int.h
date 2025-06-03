#ifndef TESTS_STORE_LOAD_INT_H_INCLUDED
#define TESTS_STORE_LOAD_INT_H_INCLUDED

void RunSet_STORE_LOAD_Int();

char* test_STORE_1_Int();
char* test_STORE_2_Int();
char* test_STORE_2_Int_AddressOutOfBounds();
char* test_LOAD_1_Int();
char* test_LOAD_3_Int();

char* test_LOAD_Int_AddressOutOfBounds();
char* test_LOAD_Num_TypeMismatch();
char* test_LOAD_Int_LoadBeforeStore();
char* test_STORE_Int_StringAlreadyStoredAtAddress();
char* test_STORE_LOAD_Int_HighestAddressableIndex();

/*The code used to implement the global storage operators is the same
* as the local operators so these tests are just to test to ensure the 
* plumbing is in place. 
*/
char* test_GSTORE_1_Int();
char* test_GLOAD_1_Int();
char* test_GLOAD_NoGlobalsDeclared();
char* test_STORE_NoLocalsDeclared();
char* test_LOAD_NoLocalsDeclared();
char* test_STORE_NoFunctionScope();
char* test_LOAD_NoFunctionScope();


#endif // TESTS_STORE_LOAD_N_H_INCLUDED
