#pragma once

#include "../VM/vm_common.h"
#include "../VM/vm_progmem_operations.h"

void static inline OpCode_SetEventHandler(vm_cpu *vm) {
	uint8_t eventId = NextCode(vm);

	//Set the handler address in the event handlers array
#ifdef DEV_ERROR_CHECKING
	if (eventId >= vm->eventHandlerCount)
		SYSTEM_ERROR(INVALID_PARAM);
#endif // ERROR_CHECKING_ENABLED

//Get address to store string version of number into
//Read handler pc location
	union TypesUnion address;
	address.bytes.low = NextCode(vm); //little endian
	address.bytes.midLow = NextCode(vm);

	vm->eventHandlers[eventId] = address.uint16;

}
