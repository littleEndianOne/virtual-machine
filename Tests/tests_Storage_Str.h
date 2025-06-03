#ifndef TESTS_STORE_LOADS_H_INCLUDED
#define TESTS_STORE_LOADS_H_INCLUDED

char* test_STORE_1_StrLit();
char* test_LOAD_1_StrRef();
char* test_STORE_1_StrRef();
char* test_STORE_1_StrLit_AddressOutOfBounds();
char* test_STORE_1_StrLit_StringTooLongForExistingAllocation();
char* test_STORE_1_StrLit_LargeExistingAllocation();

char* test_STORE_1_StrLit_NumberTypeAlreadyAtAddress();
char* test_STORE_1_StrLit_EmptyString();

char* test_STORE_1_StrRef_AddressOutOfBounds();
char* test_STORE_1_StrRef_StringTooLongForExistingAllocation();
char* test_STORE_1_StrRef_LargeExistingAllocation();
char* test_STORE_1_StrRef_NumberTypeAlreadyAtAddress();
char* test_STORE_1_StrRef_EmptyString();

char* test_GSTORE_1_StrLit();
char* test_GLOAD_1_StrRef();
char* test_GSTORE_1_StrRef();

void RunSet_STORE_LOAD_Str();


#endif // TESTS_STORE_LOAD_S_H_INCLUDED
