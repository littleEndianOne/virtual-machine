/* 
 * File:   tests_LOADAE_GLOADAE.h
 * Author: David
 *
 * Created on 28 March 2018, 8:58 PM
 */

#ifndef TESTS_LOADAE_GLOADAE_H
#define TESTS_LOADAE_GLOADAE_H

void RunSet_LOADAE_GLOADAE();

char* test_GLOADAE_Normal();
char* test_GLOADAE_VariableOutOfBounds();
char* test_GLOADAE_IndexOutOfBounds();
char* test_GLOADAE_ElementTypeNotSet();
char* test_GLOADAE_ArrayNotDeclared();

char* test_LOADAE_Normal();

char* test_GLOADAE_NoGlobals();
char* test_LOADAE_NoFuncScope();

#endif /* TESTS_STOREAE_GSTOREAE_H */

