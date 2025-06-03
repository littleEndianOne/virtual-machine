#pragma once

#include "../VM/vm_common.h"
#include "../VM/vm_opstack_operations.h"
#include "../VM/vm_progmem_operations.h"

static inline void OpCode_CONSTI(vm_cpu* vm) {
    vm_value operand;

#ifdef DEV_ERROR_CHECKING
    if (!OpCodesAvailable(vm, 4))
        SYSTEM_ERROR(PROG_SEG_FAULT);
#endif // ERROR_CHECKING_ENABLED

    operand.bytes.low = NextCode(vm); // get lowest order byte from code ...
    operand.bytes.midLow = NextCode(vm); // get next byte...
    operand.bytes.midHigh = NextCode(vm); //get next byte...
    operand.bytes.high = NextCode(vm); //get highest order byte.

    OpStackPushType(vm, INTEGER, operand); // ... and move it on top of the stack
}

static inline void OpCode_CONSTI8(vm_cpu* vm) {
    int8_t signedValue = (int8_t) NextCode(vm); // get operand from code and cast to signed type...
    vm_value operand;
    operand.int32 = (int32_t) signedValue; //Cast value to float type.
    OpStackPushType(vm, INTEGER, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTI16(vm_cpu* vm) {
    union TypesUnion signedInt;
    signedInt.uint32 = 0; // Set all bytes to 0.
    signedInt.bytes.low = NextCode(vm); // get low byte from code ...
    signedInt.bytes.midLow = NextCode(vm); //get high byte from code...
    vm_value operand;
    operand.int32 = (int32_t) signedInt.int16; //Cast value to float type.
    OpStackPushType(vm, INTEGER, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTIN0(vm_cpu* vm) {
    vm_value operand;
    operand.int32 = 0;
    OpStackPushType(vm, INTEGER, operand); //Store float value as uint32.
}

static inline void OpCode_ADDI(vm_cpu* vm) {
    
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);

    vm_value result;
    result.int32 = a.value.int32 + b.value.int32;
    
    OpStackPushType(vm, INTEGER, result); // ... add those two values and put result on top of the stack
}

static inline void OpCode_SUBI(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);

    vm_value result;
    result.int32 = a.value.int32 - b.value.int32;
    OpStackPushType(vm, INTEGER, result); // ... add those two values and put result on top of the stack
}

static inline void OpCode_MULI(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.int32 = a.value.int32 * b.value.int32;
    OpStackPushType(vm, INTEGER, result); // ... multiply those two values and put result on top of the stack
}

static inline void OpCode_DIVI(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);

#ifdef DEV_ERROR_CHECKING
    if (b.value.int32 == 0) //If
        SYSTEM_ERROR(DIV0);
#endif // ERROR_CHECKING_ENABLED

    vm_value result;
    result.int32 = a.value.int32 / b.value.int32;
    OpStackPushType(vm, INTEGER, result); // ... divide those two values and put result on top of the stack
}

static inline void OpCode_EQI(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.int32 = (a.value.int32 == b.value.int32);
    OpStackPushType(vm, INTEGER, result); // ... divide those two values and put result on top of the stack
}

static inline void OpCode_NEQI(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.int32 = (a.value.int32 != b.value.int32);
    OpStackPushType(vm, INTEGER, result); // ... divide those two values and put result on top of the stack
}

static inline void OpCode_LTI(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.int32 = (a.value.int32 < b.value.int32);
    OpStackPushType(vm, INTEGER, result); // ... divide those two values and put result on top of the stack
}

static inline void OpCode_GTI(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.int32 = (a.value.int32 > b.value.int32);
    OpStackPushType(vm, INTEGER, result); // ... divide those two values and put result on top of the stack
}

//Cast a 32bit int to a 32bit float.
static inline void OpCode_PROMOTE(vm_cpu* vm) {
    vm_element a = OpStackPop(vm);

    vm_value result;
    result.float32 = (float)a.value.int32;
    OpStackPushType(vm, FLOAT, result); // ... add those two values and put result on top of the stack
}
