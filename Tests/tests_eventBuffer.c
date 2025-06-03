#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include "tests_eventBuffer.h"
#include "../src/minunit.h"
#include "../Test Utility/vm_testing_utility.h"
#include "../VM/vm_cpu.h"
#include "../VM/vm_event_buffer.h"

void RunSet_EventBufferTests() {
    printf("\n\n\nTest Event Buffer: \n\n");
    mu_run_test(test_EventBuffer_New);
    mu_run_test(test_EventBuffer_Append_To_Empty);
    mu_run_test(test_EventBuffer_Pop_From_Empty);
    mu_run_test(test_EventBuffer_Append_And_Pop_Single);
    mu_run_test(test_EventBuffer_Append_Two);
    mu_run_test(test_EventBuffer_Append_8_Pop_4);
    mu_run_test(test_EventBuffer_Full);
    mu_run_test(test_EventBuffer_Overwrite_One_After_Wrap_Around);
    mu_run_test(test_EventBuffer_Overwrite_Full_Buffer_After_Wrap_Around);
    mu_run_test(test_EventBuffer_Over_Flow_Write_Index);
    mu_run_test(test_EventBuffer_Appened_Multiple_After_Write_Index_Overflow);
    mu_run_test(test_EventBuffer_Pop_All_From_Full);
    mu_run_test(test_EventBuffer_Pop_To_Empty_From_Half_Full);
    mu_run_test(test_EventBuffer_Pop_After_Buffer_Wrap_Around);
    mu_run_test(test_EventBuffer_Overflow_Read_Index);
    mu_run_test(test_EventBuffer_MinimumSizeBuffer_PushAndPop);
}

//Returns TRUE if overwrite occurred and FALSE if not.
boolean_t Test_EventBuffer_Append_Multiple(vm_eventBuffer* buffer, uint8_t count) {
	vm_eventBuffer_event event;
	event.param.type = INTEGER;
	event.param.value.uint32 = 0;

	boolean_t overwrite = FALSE;

	for (int i = 0; i < count; i ++){
		event.event_id = i;
		event.param.value.uint32 = i;
		overwrite = vm_EventBuffer_Append(buffer, event);
	}

	return overwrite;
}

//Return TRUE if error is found in popped events. FALSE if not.
boolean_t Test_EventBuffer_Pop_Multiple(vm_eventBuffer* buffer, uint8_t count) {
	vm_eventBuffer_event event;
	boolean_t error = FALSE;

	for (int i = 0; i < count; i ++){
		event = vm_EventBuffer_Pop(buffer);
		if (event.event_id != (int8_t)i) {
			error = TRUE;
		}
	}

	return error;
}

char* test_EventBuffer_New() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	mu_assert("Write index not set", testBuffer->write == 0);
	mu_assert("Read index not set", testBuffer->read == 0);
	mu_assert("Mask not set correctly", testBuffer->mask == 16-1);
	mu_assert("Capacity not set correctly", testBuffer->capacity == 16);
	mu_assert("Event array not created", testBuffer->array != NULL);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 0);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == TRUE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);
	vm_EventBuffer_Free(testBuffer);

    return 0;
}

char* test_EventBuffer_Append_To_Empty() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	vm_element param;
	param.type = INTEGER;
	param.value.int32 = 64;

	vm_eventBuffer_event event;
	event.event_id = 1;
	event.param = param;

	boolean_t overwrite = vm_EventBuffer_Append(testBuffer, event);

	mu_assert("Event id not correct", testBuffer->array[0].event_id == 1);
	mu_assert("Event param type not correct", testBuffer->array[0].param.type == INTEGER);
	mu_assert("Event param value not correct", testBuffer->array[0].param.value.int32 == 64);
	mu_assert("Write index incorrect", testBuffer->write == 1);
	mu_assert("Read index incorrect", testBuffer->read == 0);
	mu_assert("Overwrite not false", overwrite == FALSE);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 1);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);
	vm_EventBuffer_Free(testBuffer);

    return 0;
}

char* test_EventBuffer_Pop_From_Empty() {

		vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);
		vm_eventBuffer_event popped = vm_EventBuffer_Pop(testBuffer);
		mu_assert("popped event id incorrect", popped.event_id == -1);
		mu_assert("Write index not set", testBuffer->write == 0);
		mu_assert("Read index not set", testBuffer->read == 0);
		mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 0);
		mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == TRUE);
		mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);
		vm_EventBuffer_Free(testBuffer);

	    return 0;
}

char* test_EventBuffer_Append_And_Pop_Single() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	vm_element param;
	param.type = INTEGER;
	param.value.int32 = 64;

	vm_eventBuffer_event event;
	event.event_id = 1;
	event.param = param;

	vm_EventBuffer_Append(testBuffer, event);
	mu_assert("Event not added successfully", testBuffer->array[0].event_id == 1);

	vm_eventBuffer_event popped = vm_EventBuffer_Pop(testBuffer);

	mu_assert("Popped Event param type not correct", popped.event_id == INTEGER);
	mu_assert("Popped Event param value not correct", popped.param.value.int32 == 64);
	mu_assert("Write index incorrect", testBuffer->write == 1);
	mu_assert("Read index incorrect", testBuffer->read == 1);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 0);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == TRUE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);
	vm_EventBuffer_Free(testBuffer);

    return 0;
}

char* test_EventBuffer_Append_Two() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	vm_element param;
	param.type = INTEGER;
	param.value.int32 = 64;

	vm_eventBuffer_event event;
	event.event_id = 1;
	event.param = param;

	boolean_t overwrite = vm_EventBuffer_Append(testBuffer, event);
	event.event_id = 2;
	overwrite = vm_EventBuffer_Append(testBuffer, event);


	mu_assert("Write index incorrect", testBuffer->write == 2);
	mu_assert("Read index incorrect", testBuffer->read == 0);
	mu_assert("Overwrite not false", overwrite == FALSE);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 2);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);
	mu_assert("First Event id not correct", testBuffer->array[0].event_id == 1);
	mu_assert("Second Event id not correct", testBuffer->array[1].event_id == 2);
	vm_EventBuffer_Free(testBuffer);

    return 0;
}

char* test_EventBuffer_Append_8_Pop_4() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	//Add 16 elements
	boolean_t overwrite = Test_EventBuffer_Append_Multiple(testBuffer, 8);

	mu_assert("First event not added successfully", testBuffer->array[0].event_id == 0);
	mu_assert("Last event not added successfully", testBuffer->array[7].event_id == 7);

	mu_assert("Write index incorrect", testBuffer->write == 8);
	mu_assert("Read index incorrect", testBuffer->read == 0);
	mu_assert("Overwrite not false", overwrite == FALSE);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 8);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);

	//Remove 16 Elements
	boolean_t error = Test_EventBuffer_Pop_Multiple(testBuffer, 4);
	mu_assert("All events not popped without error", error == FALSE);

	mu_assert("Write index incorrect", testBuffer->write == 8);
	mu_assert("Read index incorrect", testBuffer->read == 4);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 4);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);

	vm_EventBuffer_Free(testBuffer);

    return 0;
}


char* test_EventBuffer_Full() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	boolean_t overwrite = Test_EventBuffer_Append_Multiple(testBuffer, 16);

	mu_assert("First event not added successfully", testBuffer->array[0].event_id == 0);
	mu_assert("Last event not added successfully", testBuffer->array[15].event_id == 15);

	mu_assert("Write index incorrect", testBuffer->write == 16);
	mu_assert("Read index incorrect", testBuffer->read == 0);
	mu_assert("Overwrite not false", overwrite == FALSE);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 16);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == TRUE);
	vm_EventBuffer_Free(testBuffer);

    return 0;
}

char* test_EventBuffer_Overwrite_One_After_Wrap_Around() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	Test_EventBuffer_Append_Multiple(testBuffer, 16);

	vm_eventBuffer_event event;
	event.event_id = 16;

	boolean_t overwrite = vm_EventBuffer_Append(testBuffer, event);

	mu_assert("First event not added successfully", testBuffer->array[0].event_id == 16);
	mu_assert("Last event not added successfully", testBuffer->array[15].event_id == 15);
	mu_assert("Overwrite not true", overwrite == TRUE);

	mu_assert("Write index incorrect", testBuffer->write == 17);
	mu_assert("Read index incorrect", testBuffer->read == 1);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 16);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == TRUE);
	vm_EventBuffer_Free(testBuffer);

    return 0;
}

char* test_EventBuffer_Overwrite_Full_Buffer_After_Wrap_Around() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	boolean_t overwrite = Test_EventBuffer_Append_Multiple(testBuffer, 32);


	mu_assert("First event not added successfully", testBuffer->array[0].event_id == 16);
	mu_assert("Last event not added successfully", testBuffer->array[15].event_id == 31);
	mu_assert("Overwrite not true", overwrite == TRUE);

	mu_assert("Write index incorrect", testBuffer->write == 32);
	mu_assert("Read index incorrect", testBuffer->read == 16);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 16);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == TRUE);
	vm_EventBuffer_Free(testBuffer);

    return 0;
}

char* test_EventBuffer_Over_Flow_Write_Index() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	Test_EventBuffer_Append_Multiple(testBuffer, 255);

	vm_eventBuffer_event event;
	event.event_id = 255;

	boolean_t overwrite = vm_EventBuffer_Append(testBuffer, event);

	mu_assert("First event not added successfully", testBuffer->array[0].event_id == (int8_t)240);
	mu_assert("Last event not added successfully", testBuffer->array[15].event_id == (int8_t)255);
	mu_assert("Overwrite not false", overwrite == TRUE);

	mu_assert("Write index incorrect", testBuffer->write == 0);
	mu_assert("Read index incorrect", testBuffer->read == 240);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 16);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == TRUE);
	vm_EventBuffer_Free(testBuffer);

    return 0;
}

char* test_EventBuffer_Appened_Multiple_After_Write_Index_Overflow() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	Test_EventBuffer_Append_Multiple(testBuffer, 255);

	vm_eventBuffer_event event;
	event.event_id = 255;

	vm_EventBuffer_Append(testBuffer, event); //overlow index by adding element 255 - index will return to 0.

	boolean_t overwrite = Test_EventBuffer_Append_Multiple(testBuffer, 4);

	mu_assert("First event not added successfully", testBuffer->array[0].event_id == 0);
	mu_assert("Last event not added successfully", testBuffer->array[3].event_id == 3);
	mu_assert("Overwrite not false", overwrite == TRUE);

	mu_assert("Write index incorrect", testBuffer->write == 4);
	mu_assert("Read index incorrect", testBuffer->read == 244);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 16);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == TRUE);
	vm_EventBuffer_Free(testBuffer);

    return 0;
}


char* test_EventBuffer_Pop_All_From_Full() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	//Add 16 elements
	boolean_t overwrite = Test_EventBuffer_Append_Multiple(testBuffer, 16);

	mu_assert("First event not added successfully", testBuffer->array[0].event_id == 0);
	mu_assert("Last event not added successfully", testBuffer->array[15].event_id == 15);

	mu_assert("Write index incorrect", testBuffer->write == 16);
	mu_assert("Read index incorrect", testBuffer->read == 0);
	mu_assert("Overwrite not false", overwrite == FALSE);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 16);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == TRUE);

	//Remove 16 Elements
	boolean_t error = Test_EventBuffer_Pop_Multiple(testBuffer, 16);
	mu_assert("All events not popped without error", error == FALSE);

	mu_assert("Write index incorrect", testBuffer->write == 16);
	mu_assert("Read index incorrect", testBuffer->read == 16);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 0);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == TRUE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);

	vm_EventBuffer_Free(testBuffer);

    return 0;
}


//Pop to empty from half full
char* test_EventBuffer_Pop_To_Empty_From_Half_Full() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	boolean_t overwrite = Test_EventBuffer_Append_Multiple(testBuffer, 8);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 8);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);

	boolean_t error = Test_EventBuffer_Pop_Multiple(testBuffer, 8);

	mu_assert("All events not popped without error", error == FALSE);

	mu_assert("Write index incorrect", testBuffer->write == 8);
	mu_assert("Read index incorrect", testBuffer->read == 8);
	mu_assert("Overwrite not false", overwrite == FALSE);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 0);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == TRUE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);


	//Pop from empty
	vm_eventBuffer_event popped = vm_EventBuffer_Pop(testBuffer);
	mu_assert("popped event id incorrect", popped.event_id == -1);
	mu_assert("Write index incorrect after pop from empty", testBuffer->write == 8);
	mu_assert("Read index incorrect after pop from empty", testBuffer->read == 8);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 0);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == TRUE);


	vm_EventBuffer_Free(testBuffer);

    return 0;
}


char* test_EventBuffer_Pop_After_Buffer_Wrap_Around() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);
	Test_EventBuffer_Append_Multiple(testBuffer, 64);

	vm_eventBuffer_event popped = vm_EventBuffer_Pop(testBuffer);
	mu_assert("Last popped event id incorrect", popped.event_id == 48);
	mu_assert("Write index incorrect", testBuffer->write == 64);
	mu_assert("Read index incorrect", testBuffer->read == 49);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 15);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);

	vm_EventBuffer_Free(testBuffer);

    return 0;
}


char* test_EventBuffer_Overflow_Read_Index() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(16);

	Test_EventBuffer_Append_Multiple(testBuffer, 255);

	boolean_t overwrite = Test_EventBuffer_Append_Multiple(testBuffer, 9);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == TRUE);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 16);
	mu_assert("Overwrite not false", overwrite == TRUE);

	Test_EventBuffer_Pop_Multiple(testBuffer, 11);
	vm_eventBuffer_event popped = vm_EventBuffer_Pop(testBuffer);
	mu_assert("Last popped event id incorrect", popped.event_id == 4);
	mu_assert("Write index incorrect", testBuffer->write == 8);
	mu_assert("Read index incorrect", testBuffer->read == 4);
	mu_assert("vm_EventBuffer_Size() not correct", vm_EventBuffer_Size(testBuffer) == 4);
	mu_assert("vm_EventBuffer_Empty() not correct", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct", vm_EventBuffer_Full(testBuffer) == FALSE);

	vm_EventBuffer_Free(testBuffer);

    return 0;
}

char* test_EventBuffer_MinimumSizeBuffer_PushAndPop() {

	vm_eventBuffer* testBuffer = vm_EventBuffer_New(1);

	//Add 16 elements
	boolean_t overwrite = Test_EventBuffer_Append_Multiple(testBuffer, 2);

	mu_assert("Write index incorrect after write", testBuffer->write == 2);
	mu_assert("Read index incorrect after write", testBuffer->read == 1);
	mu_assert("Overwrite not false after write", overwrite == TRUE);
	mu_assert("vm_EventBuffer_Size() not correct after write", vm_EventBuffer_Size(testBuffer) == 1);
	mu_assert("vm_EventBuffer_Empty() not correct after write", vm_EventBuffer_Empty(testBuffer) == FALSE);
	mu_assert("vm_EventBuffer_Full() not correct after write", vm_EventBuffer_Full(testBuffer) == TRUE);

	//Remove 1 Elements
	vm_eventBuffer_event popped = vm_EventBuffer_Pop(testBuffer);

	mu_assert("Popped event has incorrect event id", popped.event_id == 1);
	mu_assert("Write index incorrect after read", testBuffer->write == 2);
	mu_assert("Read index incorrect after read", testBuffer->read == 2);
	mu_assert("vm_EventBuffer_Size() not correct after read", vm_EventBuffer_Size(testBuffer) == 0);
	mu_assert("vm_EventBuffer_Empty() not correct after read", vm_EventBuffer_Empty(testBuffer) == TRUE);
	mu_assert("vm_EventBuffer_Full() not correct after read", vm_EventBuffer_Full(testBuffer) == FALSE);

	vm_EventBuffer_Free(testBuffer);

    return 0;
}
