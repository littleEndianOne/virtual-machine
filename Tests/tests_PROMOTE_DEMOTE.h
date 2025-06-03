/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tests_PROMOTE_DEMOTE.h
 * Author: David
 *
 * Created on 11 April 2018, 7:57 PM
 */

#ifndef TESTS_PROMOTE_DEMOTE_H
#define TESTS_PROMOTE_DEMOTE_H

void RunSet_PROMOTE_DEMOTE();

char* test_PROMOTE_integer();
char* test_PROMOTE_integer_0();
char* test_PROMOTE_integer_MaxInteger();

char* test_DEMOTE_float();
char* test_DEMOTE_float_LargeValue();

#endif /* TESTS_PROMOTE_DEMOTE_H */

