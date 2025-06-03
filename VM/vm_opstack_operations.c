#include "../VM/vm_opstack_operations.h"

#include <stdio.h>

/* Allocates memory for the passed array size and returns a pointer to the 
 * array.
 * Each array element has its type field set to the default value NOT_SET.
 * 
 * System Errors:
 * MALLOC_FAULT - Memory could not be allocated. */
vm_element* CreateOpStack(uint16_t size) {
    vm_element* stackPtr = malloc(sizeof (vm_element) * size);    
    return stackPtr; //malloc returns NULL if allocation failed.   
}

/*
 * Frees all allocated memory used for the thread opstack.
 */
void FreeOpstack(vm_element* stackPtr) {
    //Free memory allocated for opstack.     
    free(stackPtr); 
}

/*
    Pushes a stack element (variable) onto the operand stack.
    System error: STACK_OVERFLOW - Stack memory allocation is full.
 */
void inline OpStackPush(vm_cpu* vm, vm_element e) {
    ++vm->sp; //increment the stack pointer.
    #ifdef DEV_ERROR_CHECKING
    if (vm->sp == vm->stackSize)
        SYSTEM_ERROR(STACK_OVERFLOW);
    #endif // ERROR_CHECKING_ENABLED
    vm->opStack[vm->sp] = e; //Push element onto the stack
}

/*
    Pushes a stack element (variable) onto the operand stack.
    System error: STACK_OVERFLOW - Stack memory allocation is full.
 */
void OpStackPushType(vm_cpu* vm, vm_type t, vm_value v) {
    ++vm->sp; //increment the stack pointer.
    #ifdef DEV_ERROR_CHECKING
    if (vm->sp == vm->stackSize)
        SYSTEM_ERROR(STACK_OVERFLOW);
    #endif // ERROR_CHECKING_ENABLED
    vm->opStack[vm->sp].type = t;
    vm->opStack[vm->sp].value = v;
}

/*
    Pops a operand stack element (variable) from the top of the stack.
    Note: If a string type is popped the memory allocation will need to be freed by the caller.
    System error: STACK_UNDERRUN - Attempt to pop from empty stack.
*/
vm_element OpStackPop(vm_cpu* vm) {
    #ifdef DEV_ERROR_CHECKING
    if (vm->sp == -1)
        SYSTEM_ERROR(STACK_UNDERRUN);
    #endif // ERROR_CHECKING_ENABLED

    return vm->opStack[vm->sp--];
}

//Stack pop with type check
vm_value OpStackPopType(vm_cpu* vm, vm_type t) {
    vm_element e = OpStackPop(vm);

    #ifdef DEV_ERROR_CHECKING
    if (e.type != t)
        SYSTEM_ERROR(TYPE_ERROR);
    #endif // ERROR_CHECKING_ENABLED
    return e.value;
}

//Get stack element at address.
vm_element OpStackPeek(vm_cpu* vm, uint16_t address) {
    #ifdef DEV_ERROR_CHECKING
    if (address >= vm->stackSize)
        SYSTEM_ERROR(STACK_SEG_FAULT);
    #endif
    return vm->opStack[address];
}
