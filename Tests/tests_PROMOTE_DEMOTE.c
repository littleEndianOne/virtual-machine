#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_PROMOTE_DEMOTE.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_PROMOTE_DEMOTE() {
    printf("\n\n\nTest PROMOTE & DEMOTE Operators: \n\n");

    mu_run_test( test_PROMOTE_integer);
    mu_run_test( test_PROMOTE_integer_0);
    mu_run_test(test_PROMOTE_integer_MaxInteger);
    
    mu_run_test(test_DEMOTE_float);
    mu_run_test(test_DEMOTE_float_LargeValue);
}

char* test_PROMOTE_integer()
{
    int8_t pushValue = 120;

    uint8_t program[] = {
         // entrypoint - main function
        CONSTI8, pushValue,
        PROMOTE,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    
                   0,
                   4,
                   20);    
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].type == FLOAT;
    result = result & (vt->opStack[vt->sp].value.float32 == 120.00f);

    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Value not promoted correctly!", result);
    
    vm_Free(vt);
    return 0;
}

char* test_PROMOTE_integer_0()
{
    int8_t pushValue = 0;

    uint8_t program[] = {
         // entrypoint - main function
        CONSTI8, pushValue,
        PROMOTE,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    
                   0,
                   4,
                   20);    
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].type == FLOAT;
    result = result & (vt->opStack[vt->sp].value.float32 == 0.0f);

    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Value not promoted correctly!", result);
    
    vm_Free(vt);

    return 0;
}

char* test_PROMOTE_integer_MaxInteger()
{
    vm_value pushValue;
    pushValue.int32 = 2147483647;
    

    uint8_t program[] = {
         // entrypoint - main function
        CONSTI, pushValue.bytes.low, pushValue.bytes.midLow, pushValue.bytes.midHigh, pushValue.bytes.high,
        PROMOTE,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    
                   0,
                   7,
                   20);    
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].type == FLOAT;
    result = result & (vt->opStack[vt->sp].value.float32 == 2147483648.0f);  

    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Value not promoted correctly!", result);
    
    vm_Free(vt);

    return 0;
}

char* test_DEMOTE_float()
{
    vm_value pushValue;
    pushValue.float32 = 1000.5;    

    uint8_t program[] = {
         // entrypoint - main function
        CONSTF, pushValue.bytes.low, pushValue.bytes.midLow, pushValue.bytes.midHigh, pushValue.bytes.high,
        DEMOTE,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    
                   0,
                   7,
                   20);    
    vm_Run(vt); 

    uint8_t result = vt->opStack[vt->sp].type == INTEGER;
    result = result & (vt->opStack[vt->sp].value.int32 == 1000);  
    
    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Value not demoted correctly!", result);
    
    vm_Free(vt);

    return 0;
}

char* test_DEMOTE_float_LargeValue()
{
    vm_value pushValue;
    pushValue.float32 = 2000000000;    

    uint8_t program[] = {
         // entrypoint - main function
        CONSTF, pushValue.bytes.low, pushValue.bytes.midLow, pushValue.bytes.midHigh, pushValue.bytes.high,
        DEMOTE,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    
                   0,
                   7,
                   20);    
    vm_Run(vt); 

    uint8_t result = vt->opStack[vt->sp].type == INTEGER;
    result = result & (vt->opStack[vt->sp].value.int32 == 2000000000);  
    
    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Value not demoted correctly!", result);
    
    vm_Free(vt);

    return 0;
}
