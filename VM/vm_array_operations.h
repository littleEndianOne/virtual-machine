/* 
 * File:   vm_opcodes_array.h
 * Author: David
 *
 * Created on 9 March 2018, 8:35 PM
 */
#pragma once

#include "../VM/vm_common.h"

void DeclareArray(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize);
void StoreArrayRef(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element sourceArrayRef);
void LoadArrayElement(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize);
void StoreArrayElement(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize);
