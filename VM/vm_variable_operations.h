/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vm_general_store_load_functions.h
 * Author: David
 *
 * Created on 23 January 2018, 6:04 PM
 */
#pragma once

#include "../VM/vm_common.h"

//Create and destroy
vm_element* CreateVariablesArray(uint8_t varCount);
void FreeVariablesArray(vm_element* varArray, uint8_t count);

//Load
void LoadVariable(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize);
void Load(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize, uint8_t address);

//Generic Store
void StoreVariable(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize);
void Store(vm_cpu* vm, vm_element* varArray, uint8_t varArraySize, uint8_t address);

//Type implementations
void StoreFloat(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element floatE);
void StoreInt(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element intE);
void StoreStrLit(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element strLitE);
void StoreStrRef(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element srcStrRefE);
void StoreArrayRef(vm_cpu* vm, vm_element* varArray, uint8_t address, vm_element srcArrayRefE);


