#include "tests_POP.h"

#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"
#include "../VM Utility/vm_printers.h"

void RunSet_POP() {
    printf("\n\n\nTest POP Operator: \n\n");

    mu_run_test(test_POP_PushPopNumber);
    mu_run_test(test_POP_PopEmptyStack);

}

char* test_POP_PushPopNumber() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTF8, 128,
        POP,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              4, 20); // locals to be reserved, fib doesn't require them
    vm_Run(vm);

    mu_assert("Error state not as Expected", vm->errorCode == OK);
    mu_assert("Operand stack not empty!", (vm->sp == -1));

    vm_Free(vm);

    return 0;
}

char* test_POP_PopEmptyStack() {
    uint8_t program[] = {
        // entrypoint - main function        
        POP,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              2, 20); // locals to be reserved, fib doesn't require them
    vm_Run(vm);

    mu_assert("Error state not as Expected", vm->errorCode == STACK_UNDERRUN);
    mu_assert("Operand stack not empty!", (vm->sp == -1));

    vm_Free(vm);

    return 0;
}

