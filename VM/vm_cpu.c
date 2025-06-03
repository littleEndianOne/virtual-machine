#include "../VM/vm_cpu.h"

#include <stdint-gcc.h>

#include "../VM/vm_array_operations.h"
#include "../VM/vm_event_buffer.h"
#include "../VM/vm_opcodes_events.h"
#include "../VM/vm_opcodes_float.h"
#include "../VM/vm_opcodes_func.h"
#include "../VM/vm_opcodes_int.h"
#include "../VM/vm_opcodes_jump.h"
#include "../VM/vm_opcodes_stack.h"
#include "../VM/vm_opcodes_str.h"
#include "../VM/vm_opstack_operations.h"
#include "../VM/vm_progmem_operations.h"
#include "../VM/vm_string_operations.h"
#include "../VM/vm_variable_operations.h"

static void inline ExecNextOpCode(vm_cpu *vm); //Stand alone code generation for in-lined function

/*Return a pointer to an initialised vm thread.
 * Returns null if a malloc fails.
 */
vm_cpu* vm_New(uint16_t pc, uint8_t globalsCount,
		uint16_t codeSize, uint16_t stackSize, uint8_t eventHandlerCount,
		uint8_t eventBufferSize) {

	vm_cpu *vm = (vm_cpu*) malloc(sizeof(vm_cpu));

	if (vm != NULL) {
		vm->pc = pc;
		vm->localScope = NULL;
		vm->sp = -1; //-1 set empty stack
		vm->globalsCount = globalsCount;
		vm->stackSize = stackSize;
		vm->opcode = 0;
		vm->errorCode = NONE;
		vm->runState = READY;
		vm->codeSize = codeSize;
		vm->eventHandlerCount = eventHandlerCount;
		vm->eventBufferSize = eventBufferSize;

		//Initialise calbacks to NULL
		vm->systemError = NULL;
		vm->eventBufferAccessStart = NULL;
		vm->eventBufferAccessFinish = NULL;
		vm->eventBufferEmpty = NULL;

		//Initialise all globals (allocate memory and set initial values).
		if ((vm->globalsArray = CreateVariablesArray(globalsCount)) == NULL)
			return NULL;

		//Initialise the stack...
		/*Could use the CreateVariablesArray() function to initialise the stack
		 *but that would add an overhead as it initialises the type field to NONE.
		 */
		if ((vm->opStack = CreateOpStack(stackSize)) == NULL)
			return NULL;

		/*Initialise a default local scope instance so that the storage functions
		 * wont be passed a null pointer if they try to access localScope without
		 * being within a function scope.
		 */
		if ((vm->localScope = malloc(sizeof(vm_scope))) == NULL) {
			return NULL;
		} else {
			vm->initScope = vm->localScope;
			vm->localScope->argCount = 0;
			vm->localScope->localsArray = NULL;
			vm->localScope->localsCount = 0;
			vm->localScope->prevScope = NULL;
			vm->localScope->returnPC = 0;
			vm->localScope->stackFP = 0;
		}

		//Create the event handlers array
		if ((vm->eventHandlers = malloc(sizeof(uint16_t) * eventHandlerCount))
				== NULL) {
			return NULL;
		}

		//Create the event ring buffer
		vm->eventBuffer = vm_EventBuffer_New(eventBufferSize);
		if (vm->eventBuffer == NULL) {
			return NULL;
		}

		return vm;
	}
	return NULL;
}

//Free VM dynamically allocated memory

void vm_Free(vm_cpu *vm) {

	FreeVariablesArray(vm->globalsArray, vm->globalsCount);
	FreeOpstack(vm->opStack);
	//vm_EventBuffer_Free(vm->eventBuffer);

	free(vm->eventHandlers);
	free(vm->localScope);
	free(vm);
}

void inline ExecNextOpCode(vm_cpu *vm) {
	vm->opcode = NextCode(vm); // fetch next opcode
	switch (vm->opcode) {
	case HALT:
		vm->runState = HALTED;
		break;
	case ADDF:
		OpCode_ADDF(vm);
		break;
	case SUBF:
		OpCode_SUBF(vm);
		break;
	case MULF:
		OpCode_MULF(vm);
		break;
	case DIVF:
		OpCode_DIVF(vm);
		break;
	case LTF:
		OpCode_LTF(vm);
		break;
	case GTF:
		OpCode_GTF(vm);
		break;
	case EQF:
		OpCode_EQF(vm);
		break;
	case NEQF:
		OpCode_NEQF(vm);
		break;
	case ADDI:
		OpCode_ADDI(vm);
		break;
	case SUBI:
		OpCode_SUBI(vm);
		break;
	case MULI:
		OpCode_MULI(vm);
		break;
	case DIVI:
		OpCode_DIVI(vm);
		break;
	case LTI:
		OpCode_LTI(vm);
		break;
	case GTI:
		OpCode_GTI(vm);
		break;
	case EQI:
		OpCode_EQI(vm);
		break;
	case NEQI:
		OpCode_NEQI(vm);
		break;
	case PROMOTE:
		OpCode_PROMOTE(vm);
		break;
	case DEMOTE:
		OpCode_DEMOTE(vm);
		break;
	case STRLIT:
		OpCode_STRLIT(vm);
		break;
	case JMP:
		OpCode_JMP(vm);
		break;
	case JMPT:
		OpCode_JMPT(vm);
		break;
	case JMPF:
		OpCode_JMPF(vm);
		break;
	case GDSTR:
		DeclareString(vm, vm->globalsArray, vm->globalsCount);
		break;
	case GLOAD:
		LoadVariable(vm, vm->globalsArray, vm->globalsCount);
		break;
	case GSTORE:
		StoreVariable(vm, vm->globalsArray, vm->globalsCount);
		break;
	case GAPPND:
		AppendToStr(vm);
		break;
	case GCONV:
		NumToString(vm);
		break;
	case DSTR:
		DeclareString(vm, vm->localScope->localsArray,
				vm->localScope->localsCount);
		break;
	case LOAD:
		LoadVariable(vm, vm->localScope->localsArray,
				vm->localScope->localsCount);
		break;
	case STORE:
		StoreVariable(vm, vm->localScope->localsArray,
				vm->localScope->localsCount);
		break;
	case APPND:
		AppendToStr(vm);
		break;
	case TOSTR:
		NumToString(vm);
		break;
	case CALL:
		OpCode_CALL(vm);
		break;
	case RET:
		OpCode_RET(vm);
		break;
	case LDARG:
		OpCode_LDARG(vm);
		break;
	case CONSTF:
		OpCode_CONSTF(vm);
		break;
	case CONSTF8:
		OpCode_CONSTF8(vm);
		break;
	case CONSTF16:
		OpCode_CONSTF16(vm);
		break;
	case CONSTFN0:
		OpCode_CONSTFN0(vm);
		break;
	case CONSTFN1:
		OpCode_CONSTFN1(vm);
		break;
	case CONSTFN2:
		OpCode_CONSTFN2(vm);
		break;
	case CONSTFN3:
		OpCode_CONSTFN3(vm);
		break;
	case CONSTFN4:
		OpCode_CONSTFN4(vm);
		break;
	case CONSTFN5:
		OpCode_CONSTFN5(vm);
		break;
	case CONSTFN6:
		OpCode_CONSTFN6(vm);
		break;
	case CONSTFN7:
		OpCode_CONSTFN7(vm);
		break;
	case CONSTFN8:
		OpCode_CONSTFN8(vm);
		break;
	case CONSTFN9:
		OpCode_CONSTFN9(vm);
		break;
	case CONSTFN10:
		OpCode_CONSTFN10(vm);
		break;
	case CONSTI:
		OpCode_CONSTI(vm);
		break;
	case CONSTI8:
		OpCode_CONSTI8(vm);
		break;
	case CONSTI16:
		OpCode_CONSTI16(vm);
		break;
	case CONSTIN0:
		OpCode_CONSTIN0(vm);
		break;
	case POP:
		OpCode_POP(vm);
		break;
	case DARRAY:
		DeclareArray(vm, vm->localScope->localsArray,
				vm->localScope->localsCount);
		break;
	case STOREAE:
		StoreArrayElement(vm, vm->localScope->localsArray,
				vm->localScope->localsCount);
		break;
	case LOADAE:
		LoadArrayElement(vm, vm->localScope->localsArray,
				vm->localScope->localsCount);
		break;
	case GDARRAY:
		DeclareArray(vm, vm->globalsArray, vm->globalsCount);
		break;
	case GSTOREAE:
		StoreArrayElement(vm, vm->globalsArray, vm->globalsCount);
		break;
	case GLOADAE:
		LoadArrayElement(vm, vm->globalsArray, vm->globalsCount);
		break;
	case END:
		vm->runState = WAITING_FOR_EVENT;
		break;
	case SET_EVENT_HANDLER:
		OpCode_SetEventHandler(vm);
		break;
	default:
		//Inline functions are called using spare opcodes.
		//VM users must implement the InlineFunctions function which is declared in common.h
		if (!vm_ExecuteInlineFunction(vm, vm->opcode)) {
//			vm->errorState = UNKNOWN_OPCODE; //If no inline functions matching opcode then trigger fault.
			vm->runState = FAULT;
			SYSTEM_ERROR(UNKNOWN_OPCODE);
		}
		break;
	}
}

void RunStartEvent(vm_cpu *vm) {
	vm->runState = RUNNING_EVENT_HANDLER;
	while (vm->runState == RUNNING_EVENT_HANDLER) {
		ExecNextOpCode(vm);
	}
}

void RunNextEvent(vm_cpu *vm) {
	//Get next event from the buffer

	//Callback to enable system to disable interrupts during buffer access.
	if (vm->eventBufferAccessStart != NULL) {
		vm->eventBufferAccessStart(vm);
	}

	vm_eventBuffer_event nextEvent = vm_EventBuffer_Pop(vm->eventBuffer);

	//Callback to enable system to enable interrupts after access.
	if (vm->eventBufferAccessFinish != NULL) {
		vm->eventBufferAccessFinish(vm);
	}

	//event_id will be -1 if the event buffer is empty.
	if (nextEvent.event_id != -1) {
		//Push event parameter onto stack
		if (nextEvent.param.type != NONE) {
			OpStackPush(vm, nextEvent.param);
		}

		//Set program counter to handler
		vm->pc = vm->eventHandlers[nextEvent.event_id];

		//Set vm state to running
		vm->runState = RUNNING_EVENT_HANDLER;

		while (vm->runState == RUNNING_EVENT_HANDLER) {

			ExecNextOpCode(vm);
		}
	} else {
		//No event on stack.
		//Idle Callback
		if (vm->eventBufferEmpty != NULL) {
			vm->eventBufferEmpty(vm);
		}
	}
}

void vm_Run(vm_cpu *vm) {

	//A longjmp call with sys_error_env will return control flow to this location.
	int systemErrorCode = setjmp(vm->sysErrorEnv);

	/*The setjmp return value will be 0 after the initial call but return the system error code
	 value from any subsequent longjmp call.
	 */
	if (!systemErrorCode) {

		RunStartEvent(vm);

		if (vm->runState == WAITING_FOR_EVENT) {
			while (vm->runState == WAITING_FOR_EVENT) {
				RunNextEvent(vm);
			}
		}
	} else {
		//Any dynamically allocated memory must be freed here.
		vm->runState = FAULT;
		vm->errorCode = systemErrorCode;
		//Call callback function if it has been assigned.
		if (vm->systemError != NULL) {
			vm->systemError(vm);
		}
	}
}

//TODO: Add event loop into debug functions.

void vm_StartDebug(vm_cpu *vm) {
	//A longjmp call with sys_error_env will return control flow to this location.
	int SYSTEM_ERROR = setjmp(vm->sysErrorEnv);

	/*The setjmp return value will be 0 after the initial call but return the system error code
	 value from any subsequent longjmp call.
	 */
	if (!SYSTEM_ERROR) {
		vm->runState = RUNNING_EVENT_HANDLER;
	} else {
		//Any dynamically allocated memory must be freed here.
		vm->runState = FAULT;
		vm->errorCode = SYSTEM_ERROR;
		//Call callback function if it has been assigned.
		if (vm->systemError != NULL) {
			vm->systemError(vm);
		}
	}
}

/*
 Debug must be called to initialise the thread.
 Each call will execute one opcode.
 If a vm_error has occurred the function will return an error code vm_state state.
 If an opcode is executed successfully the function will return either a running or
 halted state.
 */
vm_state vm_DebugStep(vm_cpu *vm) {
	if (vm->runState == RUNNING_EVENT_HANDLER) {
		ExecNextOpCode(vm);
	} else {
		return vm->runState;
	}
	return vm->runState;
}

/*
 Debug must be called to initialise the vm.
 Will continue to execute opcodes until either a FAULT state, HALT opcode or the instCount
 is reached.
 If a vm_error has occurred the function will return an error code vm_state.
 */
vm_state vm_DebugRunFor(vm_cpu *vm, uint16_t instCount) {
	//A longjmp call with sys_error_env will return control flow to this location.
	uint16_t count = 0;
	while ((vm->runState == RUNNING_EVENT_HANDLER) & (count < instCount)) {
		ExecNextOpCode(vm);
		count++;
	}
	return vm->runState;
}

/*
 StartDebug() must be called to initialise the thread.
 Will continue to execute opcodes until either a FAULT state, HALT opcode.
 If a vm_error has occurred the function will return an error running state.
 */
vm_state vm_DebugRun(vm_cpu *vm) {
	//A longjmp call with sys_error_env will return control flow to this location.
	while (vm->runState == RUNNING_EVENT_HANDLER) {
		ExecNextOpCode(vm);
	}
	return vm->runState;
}

