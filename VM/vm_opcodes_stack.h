/*
 * Opcodes for operating on the operand stack directly.
 */

/* 
 * File:   vm_opcodes_stack.h
 * Author: David
 *
 * Created on 22 January 2018, 11:22 AM
 */
#pragma once

#include "../VM/vm_common.h"
#include "../VM/vm_opstack_operations.h"

void static inline OpCode_POP(vm_cpu* vm) {
    OpStackPop(vm);
}
