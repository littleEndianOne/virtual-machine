#pragma once

#include "../VM/vm_common.h"

char* test_EventBuffer_Mask();
char* test_EventBuffer_New();
char* test_EventBuffer_Append_To_Empty();
char* test_EventBuffer_Pop_From_Empty();
char* test_EventBuffer_Append_And_Pop_Single();
char* test_EventBuffer_Full();
char* test_EventBuffer_Append_Two();
char* test_EventBuffer_Append_8_Pop_4();
char* test_EventBuffer_Overwrite_One_After_Wrap_Around();
char* test_EventBuffer_Overwrite_Full_Buffer_After_Wrap_Around();
char* test_EventBuffer_Over_Flow_Write_Index();
char* test_EventBuffer_Appened_Multiple_After_Write_Index_Overflow();
char* test_EventBuffer_Pop_All_From_Full();
char* test_EventBuffer_Pop_To_Empty_From_Half_Full();
char* test_EventBuffer_Pop_After_Buffer_Wrap_Around();
char* test_EventBuffer_Overflow_Read_Index();
char* test_EventBuffer_MinimumSizeBuffer_PushAndPop();

boolean_t Test_EventBuffer_Append_Multiple(vm_eventBuffer* buffer, uint8_t count);
boolean_t Test_EventBuffer_Pop_Multiple(vm_eventBuffer* buffer, uint8_t count);

void RunSet_EventBufferTests();
