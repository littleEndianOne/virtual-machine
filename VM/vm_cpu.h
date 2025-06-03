#pragma once

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "../VM/vm_common.h"

//Create delete
vm_cpu* vm_New(uint16_t pc, // address of instruction to be invoked first - entrypoint/main func
		uint8_t globalsCount, //Number of global number variables to reserve.
		uint16_t codesize, //Size of addressable code memory.
		uint16_t stackSize, //The size of the operand stack to create.
		uint8_t eventHandlerCount, //The size of the array to store event handler vectors.
		uint8_t eventBufferSize); //The size of the event buffer to create (0 < size <= 2^7) and power of 2.
/*
 * Function that must be implemented to execute any inline functions.
 * Returns true if any inline functions are executed false if not.
 * Inline functions must be assigned opcodes that do not conflict with existing
 * opcodes.
 */
extern boolean_t vm_ExecuteInlineFunction(vm_cpu *vm, vm_opcode opcode);

void vm_InitGlobals(vm_cpu *vm);
void vm_Free(vm_cpu *vm);
void vm_StartDebug(vm_cpu *vm);

//Execution
void vm_Run(vm_cpu *vm);

vm_state vm_DebugStep(vm_cpu *vm);
vm_state vm_DebugRunFor(vm_cpu *vm, uint16_t instCount);
vm_state vm_DebugRun(vm_cpu *vm);
