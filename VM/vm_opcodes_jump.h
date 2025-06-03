#pragma once

#include "../VM/vm_common.h"
#include "../VM/vm_opstack_operations.h"
#include "../VM/vm_progmem_operations.h"

void static inline OpCode_JMP(vm_cpu* vm){
    union TypesUnion address;
    address.bytes.low = NextCode(vm); //little endian
    address.bytes.midLow = NextCode(vm);
    vm->pc = address.uint16;// unconditionally jump to provided address
}

void static inline OpCode_JMPT(vm_cpu* vm){
    union TypesUnion address;
    address.bytes.low = NextCode(vm); //little endian
    address.bytes.midLow = NextCode(vm);
    if(OpStackPop(vm).value.uint32) {      // ... pop value from top of the stack, and if it's true ...
        vm->pc = address.uint16; // ... jump with program counter to provided address
    }
}

void static inline OpCode_JMPF(vm_cpu* vm){
    union TypesUnion address;
    address.bytes.low = NextCode(vm); //little endian
    address.bytes.midLow = NextCode(vm);
    if(!OpStackPop(vm).value.uint32){      // ... pop value from top of the stack, and if it's true ...
        vm->pc = address.uint16; // ... jump with program counter to provided address
    }
}
