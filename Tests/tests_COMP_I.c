#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_COMP_I.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_COMP_I() {
    printf("\n\n\nTest Integer Comparison Operators: \n\n");

    mu_run_test( test_EQI_Operands_Not_Equal);
    mu_run_test( test_EQI_Operands_Equal);
    mu_run_test( test_NEQI_Operands_Not_Equal);
    mu_run_test( test_NEQI_Operands_Equal);

    mu_run_test( test_LTI_Operands_Equal);
    mu_run_test( test_LTI_Operand_A_Less_Than_B);
    mu_run_test( test_LTI_Operand_A_Greater_Than_B);
    mu_run_test( test_GTI_Operand_A_Greater_Than_B);
    mu_run_test( test_GTI_Operands_Equal);
    mu_run_test( test_GTI_Operand_A_Less_Than_B);
}

char* test_EQI_Operands_Not_Equal()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = -2000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 2000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        EQI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,
                   0,    
                   12,     //Code length
                   20);    //Stack size
    vm_Run(vt);

    uint32_t result = vt->opStack[vt->sp].value.uint32;
    
    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("Operands evaluated as equal.", result == 0);

    vm_Free(vt);

    return 0;
}

char* test_EQI_Operands_Equal()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 2000000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 2000000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        EQI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    uint32_t result = vt->opStack[vt->sp].value.uint32;
        
    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("Operands evaluated as not equal.", result == 1);

    vm_Free(vt);

    return 0;
}

char* test_NEQI_Operands_Not_Equal()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = -2000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 2000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        NEQI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    int32_t result = vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("Operands evaluated as equal.", result == 1);    

    vm_Free(vt);

    return 0;
}

char* test_NEQI_Operands_Equal()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 2000000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 2000000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        NEQI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    int32_t result = vt->opStack[vt->sp].value.uint32;

    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("Operands evaluated as not equal.", result == 0);    

    vm_Free(vt);

    return 0;
}


char* test_LTI_Operands_Equal()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 3000000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 3000000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        LTI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    int32_t result = vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("A operand evaluated as < B operand.", result == 0);   

    vm_Free(vt);

    return 0;
}

char* test_LTI_Operand_A_Less_Than_B()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 1000000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 3000000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        LTI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    int32_t result = vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("A operand not evaluated as < B operand.", result == 1);    

    vm_Free(vt);

    return 0;
}

char* test_LTI_Operand_A_Greater_Than_B()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 3000000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 1000000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        LTI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    int32_t result = vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("A operand evaluated as < B operand.", result == 0);    

    vm_Free(vt);

    return 0;
}

char* test_GTI_Operand_A_Greater_Than_B()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 3000000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 1000000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        GTI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    int32_t result = vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("A operand evaluated as < B operand.", result == 1);    

    vm_Free(vt);

    return 0;
}

char* test_GTI_Operands_Equal()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 3000000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 3000000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        GTI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    int32_t result = vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("A operand evaluated as > B operand.", result == 0);    

    vm_Free(vt);

    return 0;
}

char* test_GTI_Operand_A_Less_Than_B()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 1000000;
    union TypesUnion pushValue2;
    pushValue2.int32 = 3000000;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        GTI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);    // locals to be reserved, fib doesn't require them
    vm_Run(vt);

    int32_t result = vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK.", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0.", (vt->sp == 0));
    mu_assert("A operand not evaluated as < B operand.", result == 0);    

    vm_Free(vt);

    return 0;
}

