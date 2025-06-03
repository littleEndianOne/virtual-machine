/* 
 * File:   vm_inline_functions.c
 * Author: David
 *
 * Created on 10 March 2018, 2:02 PM
 * 
 * Dummy inline functions for testing vm execution of inline functions.
 */

#include "vm_inline_functions.h"

#include "../VM/vm_common.h"
#include "../VM/vm_opstack_operations.h"
#include "../VM/vm_progmem_operations.h"

static inline void inl_TestInline1(vm_cpu* vm)
{     
    vm_value v;
    v.float32 = 99.999;
    OpStackPushType(vm, FLOAT, v);    
}

static inline void inl_TestInline2(vm_cpu* vm)
{   
    vm_value v;
    v.float32 = 33.333;    
    OpStackPushType(vm, FLOAT, v);
}


/* Inline functions are implemented as opcodes.
 * Add switch case for each inline function opcode.
 * Function must return TRUE if a inline function runs and FALSE if not.
 */
boolean_t vm_ExecuteInlineFunction(vm_cpu* vm, vm_opcode opcode)
{
    switch ((inl_inlineFunctions)opcode)
    {
        case INLINETEST1:
            inl_TestInline1(vm);
            return TRUE;
        case INLINETEST2:
            inl_TestInline2(vm);
            return TRUE;
        default:    
            return FALSE;
    }    
}
