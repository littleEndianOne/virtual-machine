#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_CONST_F.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"

void RunSet_CONST_F() {
    printf("\n\n\nTest CONST Float load Operators: \n\n");

    mu_run_test(test_CONSTF8_PositiveValue);
    mu_run_test(test_CONSTF8_NegativeValue);
    mu_run_test(test_CONSTF16_PositiveValue);
    mu_run_test(test_CONSTF16_NegativeValue);
    mu_run_test(test_CONSTF_PositiveValue);
    mu_run_test(test_CONSTF_NegativeValue);

    mu_run_test(test_CONSTFN0);
    mu_run_test(test_CONSTFN1);
    mu_run_test(test_CONSTFN2);
    mu_run_test(test_CONSTFN3);
    mu_run_test(test_CONSTFN4);
    mu_run_test(test_CONSTFN5);
    mu_run_test(test_CONSTFN6);
    mu_run_test(test_CONSTFN7);
    mu_run_test(test_CONSTFN8);
    mu_run_test(test_CONSTFN9);
    mu_run_test(test_CONSTFN10);
}

char* test_CONSTF8_PositiveValue() {
    int8_t pushValue = 10;
    float pushValueAsFloat = (float) pushValue; //generate float representation.

    union TypesUnion typeConversion;
    typeConversion.float32 = pushValueAsFloat;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF8, pushValue,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].value.uint32 == typeConversion.uint32; //Compare value on stack to uint representation of float.

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTF8_NegativeValue() {
    int8_t pushValue = -34;
    float floatValue = -34;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF8, pushValue,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].value.float32 == floatValue; //Compare value on stack to uint representation of float.

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));

    mu_assert("Pushed value does not match value on stack!", result);

    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTF16_PositiveValue() {
    float floatValue = 2048;

    union TypesUnion converter;
    converter.int16 = 2048;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF16, converter.bytes.low, converter.bytes.midLow,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              4, 
                              20); 
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].value.float32 == floatValue; //Compare value on stack to uint representation of float.

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTF16_NegativeValue() {
    float floatValue = -3422;
    union TypesUnion converter;
    converter.int16 = -3422;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF16, converter.bytes.low, converter.bytes.midLow,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              4, 
                              20); 
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].value.float32 == floatValue; //Compare value on stack to uint representation of float.

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTF_PositiveValue() {
    float floatValue = 1999.9999;
    union TypesUnion converter;
    converter.float32 = floatValue;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF, converter.bytes.low, converter.bytes.midLow, converter.bytes.midHigh,
        converter.bytes.high,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              6, 
                              20); 
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].value.float32 == floatValue; //Compare value on stack to uint representation of float.

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;

}

char* test_CONSTF_NegativeValue() {
    float floatValue = -1999.9999;
    union TypesUnion converter;
    converter.float32 = floatValue;

    uint8_t program[] = {
        // entrypoint - main function
        CONSTF, converter.bytes.low, converter.bytes.midLow, converter.bytes.midHigh,
        converter.bytes.high,
        HALT
    };

    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              6, 
                              20); 
    vm_Run(vt);

    uint8_t result = vt->opStack[vt->sp].value.float32 == floatValue; //Compare value on stack to uint representation of float.

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;

}

char* test_CONSTFN0() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN0,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 0);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN1() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN1,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 1);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN2() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN2,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 2);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN3() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN3,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 3);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN4() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN4,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 4);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN5() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN5,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 5);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN6() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN6,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 6);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN7() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN7,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 7);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN8() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN8,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 8);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN9() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN9,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 9);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}

char* test_CONSTFN10() {
    uint8_t program[] = {
        // entrypoint - main function
        CONSTFN10,
        HALT
    };
    // initialize virtual machine
    vm_cpu* vt = TestUtil_NewThread_NoEvents(program, // program to execute
                              0, // start address of main function
                              0,
                              
                              3, 
                              20); 
    vm_Run(vt);

    uint8_t result = (vt->opStack[vt->sp].value.float32 == 10);

    mu_assert("Stack pointer not at 0!", (vt->sp == 0));
    mu_assert("Pushed value does not match value on stack!", result);
    mu_assert("Error state not OK", vt->errorCode == OK);

    vm_Free(vt);

    return 0;
}
