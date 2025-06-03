/* 
 * File:   vm_events.c
 * Author: David
 *
 * Created on 16/08/2018
 * 
 * Events implementation
 */

//#define HAVE_STRUCT_TIMESPEC
#include "../VM/vm_event_buffer.h"

#include <stdlib.h>

#include "../VM/vm_opstack_operations.h"


/*
 *NOTE: This implementation of a ring buffer can use all the allocated array slots because the index values are not
 *capped at the size of the array. They are masked into the array when events are written or read.
 *This implementation of a ring buffer works because the unsigned integer index values overflow to 0.
 *The buffer size must be a power of two and the max size of the buffer is 2^7 (which is one bit less than the indexing data type (uint8_t)
 *The index values must be of unsigned integer type.
 *The read index will only ever be a maximum of one buffer length behind the write index because when the write
 *index wraps around the array and catches up to the read index, the read index will be pushed forward (increment) when events are over written.
 *When the write index overflows back to 0 the read index will be a maximum of one buffer length from overflow also.
 */

//The buffer must be created with a capacity that is (0 < capacity <=2^7) and a power of 2.
vm_eventBuffer* vm_EventBuffer_New(uint8_t capacity) {

	//Allocate memory for the buffer

	vm_eventBuffer *newBuffer = (vm_eventBuffer*) malloc(sizeof(vm_eventBuffer));

	if (newBuffer == NULL) {
		return NULL;
	}

	newBuffer->array = (vm_eventBuffer_event*) malloc(sizeof(vm_eventBuffer_event)*capacity);

	if (newBuffer->array == NULL) {
		return NULL;
	}

	//set initial state for the buffer.
	newBuffer->read = 0;
	newBuffer->write = 0;

	newBuffer->capacity = capacity;

	//Calculate mask value for wrapping index values around the array.
	newBuffer->mask = capacity - 1;

	return newBuffer;
}

void vm_EventBuffer_Free(vm_eventBuffer *buffer){
	free(buffer->array);
	free(buffer);
}

boolean_t vm_EventBuffer_Append(vm_eventBuffer* buffer, vm_eventBuffer_event event) {

	boolean_t overwrite = FALSE;

	if (vm_EventBuffer_Full(buffer)) {
		overwrite = TRUE;
		buffer->read++; //If the buffer is overwriting an element then move the read index forward.
	}

	//Apply mask to the write index to map into the buffer.
	//The write and read index values are only mapped (mask applied) when the buffer is read or written.
	//The write index is incremented for the next write.
	vm_eventBuffer_event *selected = &buffer->array[vm_EventBuffer_Mask(buffer, buffer->write++)];

	selected->event_id = event.event_id;
	selected->param = event.param;

	return overwrite;
}


vm_eventBuffer_event vm_EventBuffer_Pop(vm_eventBuffer* buffer) {
	vm_eventBuffer_event popped;

	if (!vm_EventBuffer_Empty(buffer)) {
		//Apply mask to read index to map value into the array.
		//The read index is incremented for the next read.
		popped = buffer->array[vm_EventBuffer_Mask(buffer, buffer->read++)];
	} else {
		popped.event_id = -1; //Set -1 to indicate that buffer is empty.
	}

	return popped;
}

boolean_t vm_EventBuffer_Empty(vm_eventBuffer* buffer) {
	//The only time that the read and write index's will be equal is when the buffer is empty.
	//ie. Read index has caught up with the Write index.
	return buffer->read == buffer->write;
}

boolean_t vm_EventBuffer_Full(vm_eventBuffer* buffer) {
	return vm_EventBuffer_Size(buffer) == buffer->capacity;
}

uint8_t vm_EventBuffer_Size(vm_eventBuffer* buffer) {
	/*The read index will only ever be a maximum of one buffer length behind the write index because when the write
	 *index wraps and catches up to the read the read index will be pushed forward when events are over written.
	 *When the write index overflows back to 0 the read index will be a maximum of one buffer length from overflow,
	 *the size calculation still works in this instance because the subtraction is unsigned so instead of returning a
	 *negative value the subtraction overflows and wraps around to provide the size of the buffer.
	 */

	return buffer->write - buffer->read;
}

uint8_t vm_EventBuffer_Mask(vm_eventBuffer* buffer, uint8_t value) {
	//maps the index value that will wrap around the buffer into the buffer.
	//Depends on the buffer being a power of two.
	return (value & buffer->mask);
}






