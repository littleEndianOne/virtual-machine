#include <stdint-gcc.h>
#include <string.h>
#include "../VM Utility/vm_printers.h"
#include "tests_InlineFunctions.h"

#include "../src/minunit.h"
#include "../src/vm_inline_functions.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_InlineFunctions() {
    printf("\n\n\nTest Inline Function Execution: \n\n");
    mu_run_test(test_Inline_Test1);
    mu_run_test(test_Inline_Test1_Test2);
}

char* test_Inline_Test1() {
    uint8_t program[] = {
        // entrypoint - main function
        INLINETEST1,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // locals to be reserved, fib doesn't require them
                              2, 20); //Program length
    vm_Run(vm);
    
    float stored = vm->opStack[0].value.float32;
    float expected = 99.999;

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Operand stack not empty!", (vm->sp == 0));
    mu_assert("Stack value not as expected", stored == expected);    

    vm_Free(vm);

    return 0;
}

char* test_Inline_Test1_Test2() {
    uint8_t program[] = {
        // entrypoint - main function
        INLINETEST1,
        INLINETEST2,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vm = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0, // locals to be reserved, fib doesn't require them
                              3, 20); //Program length
    vm_Run(vm);
    
    float stored1 = vm->opStack[0].value.float32;
    float stored2 = vm->opStack[1].value.float32;
    float expected1 = 99.999;
    float expected2 = 33.333;

    mu_assert("Error state not as expected", vm->errorCode == OK);
    mu_assert("Operand stack not empty!", (vm->sp == 1));
    mu_assert("Stack value not as expected", stored1 == expected1);  
    mu_assert("Stack value not as expected", stored2 == expected2); 

    vm_Free(vm);

    return 0;
}
