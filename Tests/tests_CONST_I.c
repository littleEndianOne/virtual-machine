#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_CONST_I.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_CONST_I() {
    printf("\n\n\nTest CONST Integer load Operators: \n\n");

    mu_run_test( test_CONSTI8_positive_value);
    mu_run_test( test_CONSTI8_negative_value);
    mu_run_test( test_CONSTI8_positive_value);
    mu_run_test( test_CONSTI8_negative_value);

    mu_run_test( test_CONSTI16);
    mu_run_test( test_CONSTI16_negative_value);
    mu_run_test( test_CONSTI);
    mu_run_test( test_CONSTI_negative_value);
}

char* test_CONSTI8_positive_value()
{
    int8_t pushValue = 120;

    uint8_t program[] = {
         // entry point - main function
        CONSTI8, pushValue,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    
                   0,
                   3,
                   20);    
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].value.uint32 == pushValue;

    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);   

    vm_Free(vt);

    return 0;
}

char* test_CONSTI8_negative_value()
{
    int8_t pushValue = -120;

    uint8_t program[] = {
         // entrypoint - main function
        CONSTI8, pushValue,
        HALT
        };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   3,
                   20);    
    vm_Run(vt);
    int32_t valueFromStack = vt->opStack[vt->sp].value.int32;
    uint8_t result = valueFromStack == (int32_t)pushValue;
    
    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);

    vm_Free(vt);

    return 0;
}

char* test_CONSTI16()
{
    uint8_t pushHigh = 0x11;
    uint8_t pushLow = 0x22;
    uint16_t compValue = 0x1122;

    uint8_t program[] = {
         // entrypoint - main function
        CONSTI16, pushLow, pushHigh,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   4,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].value.uint32 == compValue;

    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    
    vm_Free(vt);

    return 0;
}

char* test_CONSTI16_negative_value()
{
    int16_t pushValue = -2400;

    union TypesUnion converter;
    converter.int16 = pushValue;

    uint8_t program[] = {
         // entrypoint - main function
        CONSTI16, converter.bytes.low, converter.bytes.midLow,
        HALT
        };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   4,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);
    int32_t valueFromStack = vt->opStack[vt->sp].value.int32;
    uint8_t result = valueFromStack == (int32_t)pushValue;
    
    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);    

    vm_Free(vt);

    return 0;
}

char* test_CONSTI()
{
    uint8_t pushHigh = 0x11;
    uint8_t pushMid = 0xFF;
    uint8_t pushLow = 0x22;
    uint32_t compValue = 0x11FFFF22;

    uint8_t program[] = {
         // entrypoint - main function
        CONSTI, pushLow, pushMid, pushMid, pushHigh,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   6,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].value.uint32 == compValue;
    
    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    
    vm_Free(vt);

    return 0;
}

char* test_CONSTI_negative_value()
{
    int32_t pushValue = -80000;

    union TypesUnion converter;
    converter.int32 = pushValue;

    uint8_t program[] = {
         // entrypoint - main function
        CONSTI, converter.bytes.low, converter.bytes.midLow, converter.bytes.midHigh, converter.bytes.high,
        HALT
        };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   6,
                   20);    
    vm_Run(vt);
    int32_t valueFromStack = vt->opStack[vt->sp].value.int32;
    uint8_t result = valueFromStack == pushValue;
    
    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);    

    vm_Free(vt);

    return 0;
}
