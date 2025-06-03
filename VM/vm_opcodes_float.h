#pragma once

#include "../VM/vm_common.h"
#include "../VM/vm_opstack_operations.h"
#include "../VM/vm_progmem_operations.h"

static inline void OpCode_CONSTF(vm_cpu* vm) {
    vm_value operand;

#ifdef DEV_ERROR_CHECKING
    if (!OpCodesAvailable(vm, 4))
        SYSTEM_ERROR(PROG_SEG_FAULT);
#endif // ERROR_CHECKING_ENABLED

    operand.bytes.low = NextCode(vm); // get lowest order byte from code ...
    operand.bytes.midLow = NextCode(vm); // get next byte...
    operand.bytes.midHigh = NextCode(vm); //get next byte...
    operand.bytes.high = NextCode(vm); //get highest order byte.
    OpStackPushType(vm, FLOAT, operand); // ... and move it on top of the stack
}

static inline void OpCode_CONSTF8(vm_cpu* vm) {
    int8_t signedValue = (int8_t) NextCode(vm); // get operand from code and cast to signed type...
    vm_value operand;
    operand.float32 = (float) signedValue; //Cast value to float type.
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTF16(vm_cpu* vm) {
    union TypesUnion signedInt;
    signedInt.uint32 = 0; // Set all bytes to 0.
    signedInt.bytes.low = NextCode(vm); // get low byte from code ...
    signedInt.bytes.midLow = NextCode(vm); //get high byte from code...
    vm_value operand;
    operand.float32 = (float) signedInt.int16; //Cast value to float type.
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN0(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 0;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN1(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 1;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN2(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 2;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN3(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 3;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN4(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 4;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN5(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 5;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN6(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 6;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN7(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 7;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN8(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 8;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN9(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 9;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_CONSTFN10(vm_cpu* vm) {
    vm_value operand;
    operand.float32 = 10;
    OpStackPushType(vm, FLOAT, operand); //Store float value as uint32.
}

static inline void OpCode_ADDF(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.float32 = a.value.float32 + b.value.float32;    
    OpStackPushType(vm, FLOAT, result); // ... add those two values and put result on top of the stack
}

static inline void OpCode_SUBF(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);

    vm_value result;
    result.float32 = a.value.float32 - b.value.float32;
    OpStackPushType(vm, FLOAT, result); // ... add those two values and put result on top of the stack
}

static inline void OpCode_MULF(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.float32 = a.value.float32 * b.value.float32;
    OpStackPushType(vm, FLOAT, result); // ... multiply those two values and put result on top of the stack
}

static inline void OpCode_DIVF(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);

#ifdef DEV_ERROR_CHECKING
    if (b.value.float32 == 0) //If
        SYSTEM_ERROR(DIV0);
#endif // ERROR_CHECKING_ENABLED

    vm_value result;
    result.float32 = a.value.float32 / b.value.float32;
    OpStackPushType(vm, FLOAT, result); // ... divide those two values and put result on top of the stack
}

static inline void OpCode_EQF(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.float32 = (a.value.float32 == b.value.float32);
    OpStackPushType(vm, FLOAT, result); // ... divide those two values and put result on top of the stack
}

static inline void OpCode_NEQF(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.float32 = (a.value.float32 != b.value.float32);
    OpStackPushType(vm, FLOAT, result); // ... divide those two values and put result on top of the stack
}

static inline void OpCode_LTF(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.float32 = (a.value.float32 < b.value.float32);
    OpStackPushType(vm, FLOAT, result); // ... divide those two values and put result on top of the stack
}

static inline void OpCode_GTF(vm_cpu* vm) {
    vm_element b = OpStackPop(vm);
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.float32 = (a.value.float32 > b.value.float32);
    OpStackPushType(vm, FLOAT, result); // ... divide those two values and put result on top of the stack
}

static inline void OpCode_DEMOTE(vm_cpu* vm) {
    vm_element a = OpStackPop(vm);
    vm_value result;
    result.int32 = (uint32_t)a.value.float32;
    OpStackPushType(vm, INTEGER, result); // ... add those two values and put result on top of the stack
}
