#include "tests_GENERAL_PROTECTION_FAULTS.h"
#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_GENERAL_PROTECTION_FAULTS.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_GeneralFaults() {
    printf("\n\n\nGeneral Faults: \n\n");
    mu_run_test(test_InvalidOpcode);
    mu_run_test(test_SystemErrorCallback);
}

char* test_InvalidOpcode() {
    int8_t pushValue = 10;

    uint8_t program[] = {
        // entrypoint - main function
        128, pushValue,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 20); // locals to be reserved, fib doesn't require them
    vm_Run(vm);

    mu_assert("Stack pointer not at -1!", (vm->sp == -1));
    mu_assert("Error state not INVALID_OPCODE", vm->errorCode == UNKNOWN_OPCODE);
    mu_assert("Run State not FAULT", vm->runState == FAULT);

    vm_Free(vm);

    return 0;
}

char* test_INVALID_CODE_ADDRESS() {
    float floatValue = -1999.9999;
    union TypesUnion converter;
    converter.float32 = floatValue;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF, converter.bytes.low, converter.bytes.midLow, converter.bytes.midHigh
    };

    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              4, 20);
    vm_Run(vm);


    //VM will continue to read bytes that fall outside the code array.
    //The random byte value will be read and a value pushed onto the op stack.
    //The next byte will most likely be an UNKNOWN_OPCODE - but not guaranteed.
    mu_assert("Error state not INVALUD_CODE_ADDRESS", vm->errorCode == PROG_SEG_FAULT);
    mu_assert("Run state not FAULT", vm->runState == FAULT);
    mu_assert("Stack pointer not at -1", (vm->sp == -1));

    vm_Free(vm);

    return 0;
}

boolean_t systemError_CallbackHandler_Called;

void systemErrorCallbackHandler() {
	systemError_CallbackHandler_Called = TRUE;
}

char* test_SystemErrorCallback() {
	systemError_CallbackHandler_Called = FALSE;

    int8_t pushValue = 10;

    uint8_t program[] = {
        // entrypoint - main function
        128, pushValue,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 20);
    vm->systemError = systemErrorCallbackHandler;

    vm_Run(vm);

    mu_assert("Stack pointer not at -1!", (vm->sp == -1));
    mu_assert("Error state not INVALID_OPCODE", vm->errorCode == UNKNOWN_OPCODE);
    mu_assert("Run State not FAULT", vm->runState == FAULT);
    mu_assert("SystemError Callback not called", systemError_CallbackHandler_Called);

    vm_Free(vm);

    return 0;
}
