#include "../Test Utility/vm_testing_utility.h"

#include <stdint-gcc.h>
#include "../VM Utility/vm_printers.h"
#include <string.h>
#include "../src/minunit.h"
//#include "../src/vm_progmem_interface.h"
#include "../VM/vm_cpu.h"

uint8_t* progmemPtr;

uint8_t vm_ReadByte(uint16_t address) {
	return progmemPtr[address];
}

char* TestFloatBinaryOperator(uint8_t op, float a, float b,
		float expectedResult, uint8_t verbose) {
	union TypesUnion operandA;
	operandA.float32 = a;
	union TypesUnion operandB;
	operandB.float32 = b;

	uint8_t program[] = {
			//Push bytes highest to lowest - Popped lowest to highest (little endian)
			CONSTF, operandA.bytes.low, operandA.bytes.midLow,
			operandA.bytes.midHigh, operandA.bytes.high, CONSTF,
			operandB.bytes.low, operandB.bytes.midLow, operandB.bytes.midHigh,
			operandB.bytes.high, op, HALT };

	// initialize virtual machine
	vm_cpu *vm = TestUtil_NewThread_NoEvents(program, // program to execute
			0, // start address of main function
			0, 12, 20);
	vm_Run(vm);

	uint8_t result = vm->opStack[vm->sp].value.float32 == expectedResult;

	if (verbose) {
		printf("\nOperand_A = %f \n", a);
		printf("Operand_B = %f \n", b);
		printf("Expected Result = %f \n", expectedResult);
		printf("Result = %f \n", vm->opStack[vm->sp].value.float32);
	}

	mu_assert("Error code not OK", vm->errorCode == OK);
	mu_assert("Stack pointer not at 0!", (vm->sp == 0));
	mu_assert("Result does not match expected!", result);

	vm_Free(vm);
	return 0;
}

vm_cpu* TestUtil_NewThread_NoEvents(uint8_t *code, // program to execute
		uint16_t pc, //initial pc value
		uint8_t globalsCount, //the number of globals to allocate memory for
		uint16_t codeSize, //Code length
		uint16_t stackSize) //Stack size
{
	vm_cpu *vt = vm_New(pc, globalsCount, codeSize, stackSize, 0, //eventHandlersCount
			0); //eventBufferSize

	//Set global progmemPtr to program code
	progmemPtr = code;

	return vt;
}

vm_cpu* TestUtil_NewThread_Events(uint8_t *code, // program to execute
		uint16_t pc, //initial pc value
		uint8_t globalsCount, //the number of globals to allocate memory for
		uint16_t codeSize, //Code length
		uint16_t stackSize, //Stack size
		uint8_t eventHandlerCount, //The size of the array to store event handler vectors.
		uint8_t eventBufferSize) //The size of the event buffer to create (0 < size <= 2^7) and power of 2.
{
	vm_cpu *vt = vm_New(pc, globalsCount, codeSize, stackSize,
			eventHandlerCount, eventBufferSize);

	//Set global progmemPtr to program code
	progmemPtr = code;

	return vt;
}

void PrintProgramAsChar(uint8_t *program, uint8_t progLength) {
	int i;

	for (i = 0; i < progLength; i++) {
		if (program[i] == '\0')
			printf("NULL ");
		else
			printf("%c ", program[i]);
	}
}

void PrintProgramAsDec(uint8_t *program, uint16_t progLength) {
	uint16_t i;

	for (i = 0; i < progLength; i++) {
		printf("%u ", program[i]);
	}
}
