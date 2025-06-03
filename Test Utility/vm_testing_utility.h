#pragma once

#include "../VM/vm_common.h"

#define FALSE 0
#define TRUE 1

char* TestFloatBinaryOperator(uint8_t op, float a, float b, float expectedResult, uint8_t verbose);

vm_cpu* TestUtil_NewThread_NoEvents(
		uint8_t *code,// program to execute
		uint16_t pc, //initial pc value
		uint8_t globalsCount, //the number of globals to allocate memory for
		uint16_t codeSize,//Code length
		uint16_t stackSize);//Stack size

vm_cpu* TestUtil_NewThread_Events(
		uint8_t *code,// program to execute
		uint16_t pc, //initial pc value
		uint8_t globalsCount, //the number of globals to allocate memory for
		uint16_t codeSize,//Code length
		uint16_t stackSize,//Stack size
		uint8_t eventHandlerCount, //The size of the array to store event handler vectors.
		uint8_t eventBufferSize); //The size of the event buffer to create (0 < size <= 2^7) and power of 2.

void PrintProgramAsChar(uint8_t *program, uint8_t progLength);
void PrintProgramAsDec(uint8_t *program, uint16_t progLength);

void testBigOrLittleEndian();
