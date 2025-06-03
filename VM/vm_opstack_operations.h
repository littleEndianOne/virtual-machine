#pragma once

#include <malloc.h>

#include "stddef.h"
#include "../VM/vm_common.h"

vm_element* CreateOpStack(uint16_t size);
void FreeOpstack(vm_element* stackPtr);

void OpStackPush(vm_cpu* vm, vm_element e);
void OpStackPushType(vm_cpu* vm, vm_type t, vm_value v);

vm_element OpStackPop(vm_cpu* vm);
vm_value  OpStackPopType(vm_cpu* vm, vm_type t);
vm_element OpStackPeek(vm_cpu* vm, uint16_t address);
