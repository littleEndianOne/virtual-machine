#pragma once

#include "../VM/vm_common.h"

//String operators

/*Read string literal from prog memory into allocated memory and push a ptr to the memory onto the
stack. */
void static inline OpCode_STRLIT(vm_cpu* vm) {
    vm_value value;

    //Get the number of characters (first byte)
    value.strLit.length = NextCode(vm);

    value.strLit.address = vm->pc; //Set address at start of characters using pc.

    //Jump pc to next opcode
    SetNextCode(vm, vm->pc + value.strLit.length);

    char term = (char) NextCode(vm); //Check for correct termination of the string.

#ifdef DEV_ERROR_CHECKING
    if (term != '\0') {
        SYSTEM_ERROR(INVALID_STRING);
    }
#endif // ERROR_CHECKING_ENABLED

    //Push string literal onto the stack
    OpStackPushType(vm, STRING_LIT, value);
}
