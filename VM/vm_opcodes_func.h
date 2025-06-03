#pragma once

#include <malloc.h>

#include "../VM/vm_common.h"
#include "../VM/vm_opstack_operations.h"
#include "../VM/vm_progmem_operations.h"
#include "../VM/vm_variable_operations.h"

/*
 * Constructs a function scope and jumps the pc to the beginning of the function code. 
 * Allocates memory for local variable storage.
 */
static inline void OpCode_CALL(vm_cpu* vm) {
    //expect all args to be on the stack
    vm_value address;

    //Read pc destination
    address.bytes.low = NextCode(vm); //little endian
    address.bytes.midLow = NextCode(vm);

    //Allocate memory for function.
    vm_scope* newScope = malloc(sizeof (vm_scope));

#ifdef DEV_ERROR_CHECKING
    if (newScope == NULL)
        SYSTEM_ERROR(MALLOC_FAULT);
#endif

    newScope->argCount = NextCode(vm); //number of args passed to function.
    newScope->localsCount = NextCode(vm); //get local storage array size.

    //Allocate memory for storing local variables.
    newScope->localsArray = CreateVariablesArray(newScope->localsCount);
    
    #ifdef DEV_ERROR_CHECKING
    if (newScope->localsArray == NULL)
        SYSTEM_ERROR(MALLOC_FAULT);
    #endif
    
    newScope->prevScope = vm->localScope; //Get previous function ptr
    vm->localScope = newScope; //Set new function as current scope

    newScope->stackFP = vm->sp; //Store current opStack address as the frame pointer.
    newScope->returnPC = vm->pc; //Store current pc for return program address.

    vm->pc = address.uint16; //Move instruction pointer to start of function code
}

/*
Retrieves a function argument from the operand stack.
Description:
Gets the index value from the prog mem.
Converts the argument index into an address that is offset relative to the stack FP address.
Fetches the value and pushes it onto the top of the operand stack.
 */
static inline void OpCode_LDARG(vm_cpu* vm) {
    // load function arg
    uint8_t index = (uint8_t) NextCode(vm); //get next value from code to identify local variables offset start on the stack

    // Check that there is something at "index" to load (Enough elements on stack).
    // If index > stackFP then calculated address would be -ve.
#ifdef DEV_ERROR_CHECKING
    if (index > vm->localScope->stackFP)
        SYSTEM_ERROR(INVALID_PARAM);
#endif  
    
    uint16_t address = vm->localScope->stackFP - index; //Calculate address.

    OpStackPush(vm, OpStackPeek(vm, address)); //Push copy of element onto the top of the stack.
}

/*
 * Return from a function call.
 * 
 * Removes any args from the operand stack 
 * Returns the VM local scope to the previous scope.
 * Frees any memory allocated for local variable storage.
 * If the function has a return value it is left on the top of the operand stack.
 */
static inline void OpCode_RET(vm_cpu* vm) {
    vm_scope* tempScope = vm->localScope; //temp store current function scope

    //Check that there is a function (scope) to return from.
#ifdef DEV_ERROR_CHECKING
    if (tempScope == vm->initScope) //initScope stores a pointer to the init scope.
        SYSTEM_ERROR(RET_NO_SCOPE);
#endif   

    vm_element rval;
    rval.type = NONE;

    if (vm->sp != tempScope->stackFP) //If SP does not match FP then there is a return value.
        rval = OpStackPop(vm); //pop return value from top of the stack before function frame change.

    /*  Return stackFP to its value before function args were pushed and the current function called.
     *  Any elements pushed onto the stack within the previous function scope that were not args will be preserved. */
    vm->sp = tempScope->stackFP - tempScope->argCount; //Set stack P to its state before args pushed and  the current function called.

    //Set the PC to the stored return point
    vm->pc = tempScope->returnPC;
    vm->localScope = tempScope->prevScope; //Return to the previous frame before the current function call.

    FreeVariablesArray(tempScope->localsArray, tempScope->localsCount); //Free memory allocated to storing locals.

    if (rval.type != NONE) //If return value has been set
        OpStackPush(vm, rval); // ... leave return value on top of the stack
}
