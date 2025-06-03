#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_MATH_I.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_MATH_I() {
    printf("\n\n\nTest Integer Math Operators: \n\n");

    mu_run_test(test_ADDI_Positive_Numbers);
    mu_run_test(test_ADDI_Negative_Numbers);
    mu_run_test(test_SUBI_NegResult);
    mu_run_test(test_MULI_Positive_Numbers);

    mu_run_test(test_MULI_Negative_Numbers);
    mu_run_test(test_MULI_Negative_And_Positive_Number);
    mu_run_test(test_DIVI);
    mu_run_test(test_DIVI_Div0);
}

char* test_ADDI_Positive_Numbers()
{
    //Brake down int into bytes.

    union TypesUnion pushValue;
    pushValue.int32 = 1024;
    int32_t expectedResult = 2048;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue.bytes.low, pushValue.bytes.midLow, pushValue.bytes.midHigh,
        pushValue.bytes.high,
        CONSTI, pushValue.bytes.low, pushValue.bytes.midLow, pushValue.bytes.midHigh,
        pushValue.bytes.high,
        ADDI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12, //Code size
                   20); //Stack size
    vm_Run(vt);

    int32_t actualResult = vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Add result does not match expected!", actualResult == expectedResult);   

    vm_Free(vt);
    return 0;
}

char* test_ADDI_Negative_Numbers()
{
    //Brake down int into bytes.

    union TypesUnion pushValue;
    pushValue.int32 = -1024;
    int32_t expectedResult = -2048;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue.bytes.low, pushValue.bytes.midLow, pushValue.bytes.midHigh,
        pushValue.bytes.high,
        CONSTI, pushValue.bytes.low, pushValue.bytes.midLow, pushValue.bytes.midHigh,
        pushValue.bytes.high,
        ADDI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);
    vm_Run(vt);

    uint32_t actualResult = vt->opStack[vt->sp].value.int32;
    
    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Add result does not match expected!", actualResult == expectedResult);    

    vm_Free(vt);
    return 0;
}

char* test_SUBI_NegResult()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 1024;
    union TypesUnion pushValue2;
    pushValue2.int32 = 2048;
    int32_t expectedResult = -1024;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        SUBI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);
    vm_Run(vt);

    int32_t actualResult = (int32_t)vt->opStack[vt->sp].value.int32;
    
    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Result does not match expected!", actualResult == expectedResult);    

    vm_Free(vt);
    return 0;
}


char* test_DIVI()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 4096;
    union TypesUnion pushValue2;
    pushValue2.int32 = 4;
    int32_t expectedResult = 1024;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        DIVI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);
    vm_Run(vt);

    int32_t actualResult = (int32_t)vt->opStack[vt->sp].value.int32;
    
    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("DIV result does not match expected!", actualResult == expectedResult);    

    vm_Free(vt);
    return 0;
}

char* test_DIVI_Div0()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 4096;
    union TypesUnion pushValue2;
    pushValue2.int32 = 0;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        DIVI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);
    vm_Run(vt);
      
    mu_assert("Error state not DIV0", vt->errorCode == DIV0);
    mu_assert("Stack pointer not at 0!", (vt->sp == -1));    

    vm_Free(vt);
    return 0;
}

char* test_MULI_Positive_Numbers()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = 1024;
    union TypesUnion pushValue2;
    pushValue2.int32 = 4;
    int32_t expectedResult = 4096;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        MULI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);
    vm_Run(vt);

    int32_t actualResult = (int32_t)vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Result does not match expected!", actualResult == expectedResult);
    
    vm_Free(vt);
    return 0;
}

char* test_MULI_Negative_Numbers()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = -1024;
    union TypesUnion pushValue2;
    pushValue2.int32 = -4;
    int32_t expectedResult = 4096;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        MULI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);
    vm_Run(vt);

    int32_t actualResult = (int32_t)vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Result does not match expected!", actualResult == expectedResult);
    
    vm_Free(vt);
    return 0;
}

char* test_MULI_Negative_And_Positive_Number()
{
    //Brake down int into bytes.

    union TypesUnion pushValue1;
    pushValue1.int32 = -1024;
    union TypesUnion pushValue2;
    pushValue2.int32 = 4;
    int32_t expectedResult = -4096;

    uint8_t program[] = {
        //Push bytes highest to lowest - Popped lowest to highest (little endian)
        CONSTI, pushValue1.bytes.low, pushValue1.bytes.midLow, pushValue1.bytes.midHigh,
        pushValue1.bytes.high,
        CONSTI, pushValue2.bytes.low, pushValue2.bytes.midLow, pushValue2.bytes.midHigh,
        pushValue2.bytes.high,
        MULI,
        HALT
        };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program,   // program to execute
                   0,    // start address of main function
                   0,
                   12,
                   20);
    vm_Run(vt);

    int32_t actualResult = (int32_t)vt->opStack[vt->sp].value.int32;

    mu_assert("Error state not OK", vt->errorCode == OK);
    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Result does not match expected!", actualResult == expectedResult);
    
    vm_Free(vt);
    return 0;
}





