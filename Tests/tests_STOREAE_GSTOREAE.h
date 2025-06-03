/* 
 * File:   tests_STOREAE_GSTOREAE.h
 * Author: David
 *
 * Created on 27 March 2018, 8:58 PM
 */

#ifndef TESTS_STOREAE_GSTOREAE_H
#define TESTS_STOREAE_GSTOREAE_H

void RunSet_STOREAE_GSTOREAE();

char* test_GSTOREAE_Normal();
char* test_GSTOREAE_VariableOutOfBounds();
char* test_GSTOREAE_IndexOutOfBounds();
char* test_GSTOREAE_IncorectType();
char* test_STOREAE_Normal();

char* test_GSTOREAE_NoGlobals();
char* test_STOREAE_NoFuncScope();

#endif /* TESTS_STOREAE_GSTOREAE_H */