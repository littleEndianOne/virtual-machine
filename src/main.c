#include <stdint-gcc.h>
#include <stdio.h>

#include "../Tests/tests_APPEND_GAPPEND.h"
#include "../Tests/tests_CALL_RET.h"
#include "../Tests/tests_COMP_F.h"
#include "../Tests/tests_COMP_I.h"
#include "../Tests/tests_CONST_F.h"
#include "../Tests/tests_CONST_I.h"
#include "../Tests/tests_CONV.h"
#include "../Tests/tests_DARAY_GDARAY.h"
#include "../Tests/tests_DSTR_GDSTR.h"
#include "../Tests/tests_Events.h"
#include "../Tests/tests_GENERAL_PROTECTION_FAULTS.h"
#include "../Tests/tests_JUMP.h"
#include "../Tests/tests_LOADAE_GLOADAE.h"
#include "../Tests/tests_MATH_F.h"
#include "../Tests/tests_MATH_I.h"
#include "../Tests/tests_InlineFunctions.h"
#include "../Tests/tests_PROMOTE_DEMOTE.h"
#include "../Tests/tests_stack_operations.h"
#include "../Tests/tests_Storage_Float.h"
#include "../Tests/tests_Storage_Int.h"
#include "../Tests/tests_Storage_Str.h"
#include "../Tests/tests_STOREAE_GSTOREAE.h"
#include "../Tests/tests_STRLIT.h"
#include "../Tests/tests_eventBuffer.h"
#include "../Tests/tests_Events.h"
#include "../VM/vm_common.h"
#include "vm_inline_functions.h"

int tests_run;
int tests_passed;
int tests_failed;

void ShowTestStats();
void RunAllSets();

/*Implement program memory read that accesses an array pointed to from within
 *the vm. It is implemented this way because the test cases were written 
 *before the external ReadByte function was implemented.
 */


int main() {
    //RunSet_POP();

    //RunSet_CONSTF();

    //RunSet_MATH_F();   

    //RunSet_COMP_F();

    //RunSet_CONST_I(); 

    //RunSet_MATH_I();

    //RunSet_COMP_I();

    //RunSet_STRLIT();    

    //RunSet_APPEND();

    //RunSet_CONV();

    //RunSet_JUMP();

    //RunSet_CALL();

    //RunSet_STORE_LOAD_Float();

    //RunSet_STORE_LOAD_Int();

    //RunSet_STORE_LOADS();

    //RunSet_GSTORE_GLOADS();

    //RunSet_DSTR_GDSTR();

    //RunSet_StackOperations();

    //RunSet_GeneralFaults();

    //RunSet_InlineFunctions();

    //RunSet_Printers();

    //RunSet_DARY_GDARAY();

    //RunSet_STOREAE_GSTOREAE();

    //RunSet_LOADAE_GLOADAE();

    //RunSet_PROMOTE_DEMOTE();

    //RunSet_EventBufferTests();

	//RunSet_Events();

    RunAllSets();

    ShowTestStats();

    return tests_failed == 0 ? 0 : 1;
}

void RunAllSets() {
    printf("Test VM Instructions: \n");
    printf("---------------------------------\n\n");

    RunSet_CONST_F();

    RunSet_MATH_F();

    RunSet_COMP_F();

    RunSet_CONST_I();

    RunSet_MATH_I();

    RunSet_COMP_I();

    RunSet_STRLIT();

    RunSet_STORE_LOAD_Str();

    RunSet_DSTR_GDSTR();

    RunSet_APPEND();

    RunSet_CONV();

    RunSet_JUMP();

    RunSet_CALL();

    RunSet_STORE_LOAD_Float();

    RunSet_STORE_LOAD_Int();

    RunSet_StackOperations();

    RunSet_GeneralFaults();

    RunSet_InlineFunctions();

    RunSet_DARY_GDARAY();

    RunSet_STOREAE_GSTOREAE();

    RunSet_LOADAE_GLOADAE();

    RunSet_PROMOTE_DEMOTE();

    RunSet_EventBufferTests();

    RunSet_Events();
}

void ShowTestStats() {
    printf("\n\n\nMetrics: \n");
    printf("Tests run: %d \n", tests_run);
    printf("Tests passed: %d \n", tests_passed);
    printf("Tests failed: %d \n", tests_failed);
    printf("Percentage tests failed: %f %%\n", ((float) tests_failed / tests_run)* 100);
}

