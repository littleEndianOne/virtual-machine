/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vm_events.h
 * Author: David
 * 
 * Definition of events interface for vm
 *
 * Created on 16/08/19
 */

#pragma once

#include "../VM/vm_common.h"


//Size must be a power of two and no bigger than 128 (half of the max value of index type i.e. uint8_t.
vm_eventBuffer* vm_EventBuffer_New(uint8_t size);


void vm_EventBuffer_Free(vm_eventBuffer *buffer);

//Returns oldest event or invalid event with event.id = -1 if the buffer is empty.
vm_eventBuffer_event vm_EventBuffer_Pop(vm_eventBuffer* buffer);

//Overwrites when the buffer is full. Returns true if overwrite occurs...
boolean_t vm_EventBuffer_Append(vm_eventBuffer* buffer, vm_eventBuffer_event event);

//Returns true if the buffer is empty
boolean_t vm_EventBuffer_Empty(vm_eventBuffer* buffer);

//Returns true if the buffer is full
boolean_t vm_EventBuffer_Full(vm_eventBuffer* buffer);

//Returns the number of events currently stored within the buffer
uint8_t vm_EventBuffer_Size(vm_eventBuffer* buffer);

//Applies the mask value to the index value to map the index value into the ring buffer.
//The write and read index values are only mapped (mask applied) when the buffer is read or written.
//The index values will also overflow to 0.
uint8_t vm_EventBuffer_Mask(vm_eventBuffer* buffer, uint8_t value);
