#pragma once
/* 
 * File:   tests_InlineFunctions.h
 * Author: David
 *
 * Created on 10 March 2018, 3:39 PM
 */

#include "../VM/vm_common.h"

void RunSet_Events();

char* test_SetEventHandler();
char* test_SetEventHandler_Id_Out_Of_Range();

void test_SingleEvent_With_Parameters_eventBufferAccessFinishHandler(vm_cpu *vm);
char* test_SingleEvent_With_Parameter();

char* test_EventCallbacks();

void test_MultiplEvents_With_Parameters_eventBufferAccessFinishHandler(vm_cpu *vm);
char* test_MultiplEvents_With_Parameters();

void test_RunTwoBatchesOfEvents_First_eventBufferAccessFinishHandler(vm_cpu *vm);
void test_RunTwoBatchesOfEvents_Second_eventBufferAccessFinishHandler(vm_cpu *vm);
char* test_RunTwoBatchesOfEvents_No_Parameters();

char* test_OutOfRange_Event_Handler_Address();
char* test_SystemError_In_Handler();

void FireTestEvent_0(vm_cpu *vm, int32_t param);
void AppenedTestEvent_IntParam(vm_cpu *vm, uint8_t id, int32_t param);
