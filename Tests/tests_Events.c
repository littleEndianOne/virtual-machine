#include <stdint-gcc.h>
#include <string.h>

//#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#include "../VM Utility/vm_printers.h"
#include "tests_InlineFunctions.h"
#include "tests_Events.h"

#include "../src/minunit.h"
#include "../src/vm_inline_functions.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_common.h"
#include "../VM/vm_cpu.h"
#include "../VM/vm_event_buffer.h"

void RunSet_Events() {
	printf("\n\n\nTest Events: \n\n");
	//mu_run_test(test_TwoEvents);
	mu_run_test(test_SetEventHandler);
	mu_run_test(test_SetEventHandler_Id_Out_Of_Range);
	mu_run_test(test_SingleEvent_With_Parameter);
	mu_run_test(test_EventCallbacks);
	mu_run_test(test_MultiplEvents_With_Parameters);
	mu_run_test(test_RunTwoBatchesOfEvents_No_Parameters);
	mu_run_test(test_OutOfRange_Event_Handler_Address);
	mu_run_test(test_SystemError_In_Handler);
}

void* runVM(void *vm) {
	printf("VM Running in thread!!!\n");
	vm_Run(((vm_cpu*) vm));
	return NULL;
}

char* test_SetEventHandler() {

	uint8_t program[] = {
	// entrypoint - main function
			SET_EVENT_HANDLER, 0, 1, 1, HALT, };

	// initialize virtual machine

	vm_cpu *vm = TestUtil_NewThread_Events(program, 0, // start address of main function
			0, //Globals count
			5, //code size
			8, //stack size
			2, //eventHandlersCount
			16); //eventBufferSize

	vm_Run(vm);

	mu_assert("Error state not as expected", vm->errorCode == OK);
	mu_assert("Operand stack not empty!", (vm->sp == -1));
	mu_assert("Event handler not set correctly", vm->eventHandlers[0] == 257);

	vm_Free(vm);

	return 0;
}

char* test_SetEventHandler_Id_Out_Of_Range() {

	uint8_t program[] = {
	// Entry point - main function
			SET_EVENT_HANDLER, 3, 1, 1, HALT, };

	// initialize virtual machine

	vm_cpu *vm = TestUtil_NewThread_Events(program, 0, // start address of main function
			0, //Globals count
			5, //code size
			8, //stack size
			2, //eventHandlersCount
			16); //eventBufferSize

	vm_Run(vm);

	mu_assert("Error code not as expected", vm->errorCode == INVALID_PARAM);
	mu_assert("Operand stack not empty!", (vm->sp == -1));

	vm_Free(vm);

	return 0;
}

void AppenedTestEvent_IntParam(vm_cpu *vm, uint8_t id, int32_t param) {
	vm_eventBuffer_event event;
	event.event_id = id;
	event.param.type = INTEGER;
	event.param.value.int32 = param;
	vm_EventBuffer_Append(vm->eventBuffer, event);
}

void AppenedTestEvent_NoParam(vm_cpu *vm, uint8_t id) {
	vm_eventBuffer_event event;
	event.event_id = id;
	event.param.type = NONE;
	vm_EventBuffer_Append(vm->eventBuffer, event);
}


void test_SingleEvent_With_No_Parameter_eventBufferAccessFinishHandler(vm_cpu *vm) {
	AppenedTestEvent_NoParam(vm, 0);
	vm->eventBufferAccessFinish = NULL;
}

char* test_SingleEvent_With_No_Parameter() {

	uint8_t program[] = {
	// entrypoint - main function
			SET_EVENT_HANDLER, 0, 5, 0, END, HALT };

	// initialize virtual machine
	vm_cpu *vm = TestUtil_NewThread_Events(program, 0, // start address of main function
			0, //Globals count
			6, //code size
			8, //stack size
			2, //eventHandlersCount
			16); //eventBufferSize

	//Set eventBufferAccessFinish
	//Add an event to the event stack after the vm completes its first check of the buffer.
	//The next event loop will execute the event.
	vm->eventBufferAccessFinish =
			test_SingleEvent_With_No_Parameter_eventBufferAccessFinishHandler;

	vm_Run(vm);

	mu_assert("Error state not as expected", vm->errorCode == OK);
	mu_assert("Operand stack not empty!", (vm->sp == -1));

	vm_Free(vm);

	return 0;
}

void test_SingleEvent_With_Parameters_eventBufferAccessFinishHandler(vm_cpu *vm) {
	AppenedTestEvent_IntParam(vm, 0, 2);
	vm->eventBufferAccessFinish = NULL;
}

char* test_SingleEvent_With_Parameter() {

	uint8_t program[] = {
	// entrypoint - main function
			SET_EVENT_HANDLER, 0, 5, 0, END, CONSTI8, 3, HALT };

	// initialize virtual machine
	vm_cpu *vm = TestUtil_NewThread_Events(program, 0, // start address of main function
			0, //Globals count
			8, //code size
			8, //stack size
			2, //eventHandlersCount
			16); //eventBufferSize

	//Set eventBufferAccessFinish
	//Add an event to the event stack after the vm completes its first check of the buffer.
	//The next event loop will execute the event.
	vm->eventBufferAccessFinish =
			test_SingleEvent_With_Parameters_eventBufferAccessFinishHandler;

	vm_Run(vm);

	uint8_t param = vm->opStack[0].value.uint8;
	uint8_t push = vm->opStack[1].value.uint8;

	mu_assert("Error state not as expected", vm->errorCode == OK);
	mu_assert("Stack element value not as expected", param == 2);
	mu_assert("Stack element value not as expected", push == 3);
	mu_assert("Element not on operand stack!", (vm->sp == 1));

	vm_Free(vm);

	return 0;
}

boolean_t eventBufferAccessFinishHandlerCalled;
boolean_t eventBufferAccessStartHandlerCalled;
boolean_t eventBufferEmptyHandlerCalled;

void test_EventCallbacks_eventBufferAccessFinishHandler(vm_cpu *vm) {
	eventBufferAccessFinishHandlerCalled = TRUE;

}

void test_EventCallbacks_eventBufferAccessStartHandler(vm_cpu *vm) {
	eventBufferAccessStartHandlerCalled = TRUE;
}

void test_EventCallbacks_eventBufferEmptyHandler(vm_cpu *vm) {
	//Append an event to the empty buffer
	AppenedTestEvent_IntParam(vm, 0, 32);
	eventBufferEmptyHandlerCalled = TRUE;
	//Clear the handler so that additional events are not added to the buffer.
	//The vm event handler will halt the vm.
	vm->eventBufferEmpty = NULL;
}

char* test_EventCallbacks() {
	eventBufferAccessFinishHandlerCalled = FALSE;
	eventBufferAccessStartHandlerCalled = FALSE;
	eventBufferEmptyHandlerCalled = FALSE;

	uint8_t program[] = {
	// entrypoint - main function
			SET_EVENT_HANDLER, 0, 5, 0,
			END,
			HALT };

	// initialize virtual machine
	vm_cpu *vm = TestUtil_NewThread_Events(program, 0, // start address of main function
			0, //Globals count
			8, //code size
			8, //stack size
			2, //eventHandlersCount
			16); //eventBufferSize


	vm->eventBufferAccessFinish = test_EventCallbacks_eventBufferAccessFinishHandler;
	vm->eventBufferAccessStart = test_EventCallbacks_eventBufferAccessStartHandler;
	vm->eventBufferEmpty = test_EventCallbacks_eventBufferEmptyHandler;

	vm_Run(vm);

	uint8_t param = vm->opStack[0].value.uint8;

	mu_assert("AccessFinish Handler not called", eventBufferAccessFinishHandlerCalled);
	mu_assert("AccessStart Handler not called", eventBufferAccessStartHandlerCalled);
	mu_assert("EventBufferEmpty Handler not called", eventBufferEmptyHandlerCalled);

	mu_assert("Error state not as expected", vm->errorCode == OK);
	mu_assert("Stack element value not as expected", param == 32);
	mu_assert("Element not on operand stack!", (vm->sp == 0));

	vm_Free(vm);

	return 0;
}

void test_MultiplEvents_With_Parameters_eventBufferAccessFinishHandler(
		vm_cpu *vm) {

	for (int i = 0; i < 3; i++) {
		AppenedTestEvent_IntParam(vm, i, i);
	}
	vm->eventBufferAccessFinish = NULL;
}

char* test_MultiplEvents_With_Parameters() {

	uint8_t program[] = {
			// entrypoint - main function
			SET_EVENT_HANDLER, 0, 13, 0, SET_EVENT_HANDLER, 1, 17, 0,
			SET_EVENT_HANDLER, 2, 19, 0, END, CONSTI8, 7, ADDI, END, ADDI, END,
			MULI, HALT };

	// initialize virtual machine
	vm_cpu *vm = TestUtil_NewThread_Events(program, 0, // start address of main function
			0, //Globals count
			21, //code size
			8, //stack size
			3, //eventHandlersCount
			16); //eventBufferSize

	//Set eventBufferAccessFinish
	//Add an event to the event stack after the vm completes its first check of the buffer.
	//The next event loop will execute the event.
	vm->eventBufferAccessFinish =
			test_MultiplEvents_With_Parameters_eventBufferAccessFinishHandler;

	vm_Run(vm);

	int32_t stored = vm->opStack[0].value.int32;

	mu_assert("Error state not as expected", vm->errorCode == OK);
	mu_assert("Stack element value not as expected", stored == 16);
	mu_assert("Unexpected stack pointer value!", (vm->sp == 0));

	vm_Free(vm);

	return 0;
}

void test_RunTwoBatchesOfEvents_First_eventBufferAccessFinishHandler(vm_cpu *vm) {

	for (int i = 0; i < 8; i++) {
		vm_eventBuffer_event event;
		event.event_id = 0;
		event.param.type = NONE;
		vm_EventBuffer_Append(vm->eventBuffer, event);
	}

	vm->eventBufferEmpty = test_RunTwoBatchesOfEvents_Second_eventBufferAccessFinishHandler;
}

void test_RunTwoBatchesOfEvents_Second_eventBufferAccessFinishHandler(
		vm_cpu *vm) {

		for (int i = 0; i < 4; i++) {
			vm_eventBuffer_event event;
			event.event_id = 1;
			event.param.type = NONE;
			vm_EventBuffer_Append(vm->eventBuffer, event);
		}

		//Add final event to halt vm
		vm_eventBuffer_event finalEvent;
		finalEvent.event_id = 2;
		finalEvent.param.type = NONE;
		vm_EventBuffer_Append(vm->eventBuffer, finalEvent);

		vm->eventBufferEmpty = NULL;
}

char* test_RunTwoBatchesOfEvents_No_Parameters() {

	uint8_t program[] = {
			// entrypoint - main function
			CONSTI8, 0, GSTORE, 0, CONSTI8, 0, GSTORE, 1, SET_EVENT_HANDLER, 0,
			26, 0, SET_EVENT_HANDLER, 1, 34, 0, SET_EVENT_HANDLER, 2, 21, 0,
			END, GLOAD, 0, GLOAD, 1, HALT, GLOAD, 0, CONSTI8, 1, ADDI, GSTORE,
			0, END, GLOAD, 1, CONSTI8, 1, ADDI, GSTORE, 1, END, };

	// initialize virtual machine
	vm_cpu *vm = TestUtil_NewThread_Events(program, 0, // start address of main function
			2, //Globals count
			42, //code size
			8, //stack size
			3, //eventHandlersCount
			16); //eventBufferSize

	//Set the call back that will set the first batch of events to process.
	//The callback will set the callback for the second batch.
	vm->eventBufferEmpty =
			test_RunTwoBatchesOfEvents_First_eventBufferAccessFinishHandler;

	vm_Run(vm);

	mu_assert("Error state not as expected", vm->errorCode == OK);
	mu_assert("Unexpected stack pointer value!", (vm->sp == 1));
	mu_assert("Global 0 value not as expected",
			(vm->opStack[0].value.int32) == 8);
	mu_assert("Global 1 value not as expected",
			(vm->opStack[1].value.int32) == 4);

	vm_Free(vm);

	return 0;
}

void test_Invalid_Event_Handler_Address_eventBufferAccessFinishHandler(vm_cpu *vm) {
	AppenedTestEvent_NoParam(vm, 0);
	vm->eventBufferAccessFinish = NULL;
}

char* test_OutOfRange_Event_Handler_Address() {

	uint8_t program[] = {
	// entrypoint - main function
			SET_EVENT_HANDLER, 0, 8, 0, END, HALT };

	// initialize virtual machine
	vm_cpu *vm = TestUtil_NewThread_Events(program, 0, // start address of main function
			0, //Globals count
			6, //code size
			8, //stack size
			2, //eventHandlersCount
			16); //eventBufferSize

	//Set eventBufferAccessFinish
	//Add an event to the event stack after the vm completes its first check of the buffer.
	//The next event loop will execute the event.
	vm->eventBufferEmpty =
			test_SingleEvent_With_No_Parameter_eventBufferAccessFinishHandler;

	vm_Run(vm);

	mu_assert("Run state not as expected", vm->runState == FAULT);
	mu_assert("Error code not as expected", vm->errorCode == PROG_SEG_FAULT);

	vm_Free(vm);

	return 0;
}

void test_test_SystemError_In_Handler_eventBufferAccessFinishHandler(vm_cpu *vm) {
	AppenedTestEvent_NoParam(vm, 0);
	vm->eventBufferAccessFinish = NULL;
}

char* test_SystemError_In_Handler() {

	uint8_t program[] = {
	// entrypoint - main function
			SET_EVENT_HANDLER, 0, 5, 0, END, ADDI };

	// initialize virtual machine
	vm_cpu *vm = TestUtil_NewThread_Events(program, 0, // start address of main function
			0, //Globals count
			6, //code size
			8, //stack size
			2, //eventHandlersCount
			16); //eventBufferSize

	//Set eventBufferAccessFinish
	//Add an event to the event stack after the vm completes its first check of the buffer.
	//The next event loop will execute the event.
	vm->eventBufferEmpty =
			test_SingleEvent_With_No_Parameter_eventBufferAccessFinishHandler;

	vm_Run(vm);

	mu_assert("Run state not as expected", vm->runState == FAULT);
	mu_assert("Error code not as expected", vm->errorCode == STACK_UNDERRUN);

	vm_Free(vm);

	return 0;
}
