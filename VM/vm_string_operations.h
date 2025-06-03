/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vm_string_operations.h
 * Author: David
 *
 * Created on 10 April 2018, 9:06 PM
 */

#pragma once

#include "../VM/vm_common.h"

void AppendToStr(vm_cpu* vm);
void DeclareString(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize);
void NumToString(vm_cpu* vm);
void IntToString(int32_t number, char* str, uint8_t strSize);

void ReadStringLit(vm_cpu* vm, vm_strlit strLit, char* dest, uint8_t strAllocSize);
void DuplicateString(char* src, char* dest, uint8_t allocLength);
