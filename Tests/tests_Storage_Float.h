#ifndef TESTS_STORE_LOAD_FLOAT_H_INCLUDED
#define TESTS_STORE_LOAD_FLOAT_H_INCLUDED

char* test_STORE_1_Float();
char* test_STORE_2_Float();
char* test_STORE_2_Float_AddressOutOfBounds();
char* test_LOAD_1_Flaot();
char* test_LOAD_3_Float();
char* test_LOAD_Float_AddressOutOfBounds();
char* test_LOAD_Float_TypeMismatch();
char* test_LOAD_Float_LoadBeforeStore();
char* test_STORE_Float_StringAlreadyStoredAtAddress();

char* test_STORE_LOAD_Float_HighestAddressableIndex();
void RunSet_STORE_LOAD_Float();

/*The code used to implement the global storage operators is the same
* as the local operators so these tests are just to test to ensure the 
* plumbing is in place. 
*/
char* test_GSTORE();
char* test_GLOAD();


#endif // TESTS_STORE_LOAD_N_H_INCLUDED
