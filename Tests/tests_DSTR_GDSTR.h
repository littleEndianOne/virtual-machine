/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tests_STR_GSTR.h
 * Author: David
 *
 * Created on 26 January 2018, 12:51 PM
 */

#ifndef TESTS_STR_GSTR_H
#define TESTS_STR_GSTR_H

char* test_DSTR_TypicalString();
char* test_DSTR_MaxLengthString();
char* test_DSTR_ZeroLengthAllocation();
char* test_DSTR_MinLengthString();

char* test_GDSTR_TypicalString();
char* test_DSTR_NoFuncScope();

void RunSet_DSTR_GDSTR();

#endif /* TESTS_STR_GSTR_H */

