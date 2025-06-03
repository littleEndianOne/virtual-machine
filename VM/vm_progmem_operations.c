#include "../VM/vm_progmem_operations.h"

/* Get next opcode and advance pc.
 * Extern inline declaration to provide symbol for non in-lined calls.
 * See header file for more details.
 */
uint8_t NextCode(vm_cpu* vm) {
    #ifdef DEV_ERROR_CHECKING
    if (vm->pc >= vm->codeSize)
        SYSTEM_ERROR(PROG_SEG_FAULT);
    #endif // ERROR_CHECKING_ENABLED
    
    return vm_ReadByte(vm->pc++);
}

// Get next opcode without advancing PC.

int8_t PeakNextCode(vm_cpu* vm) {
    return vm_ReadByte(vm->pc);
}

//Return the opcode at the passed address.

uint8_t PeakCodeAt(vm_cpu* vm, uint16_t address) {
    #ifdef DEV_ERROR_CHECKING
    if (address >= vm->codeSize)
        SYSTEM_ERROR(PROG_SEG_FAULT);
    #endif // ERROR_CHECKING_ENABLED
    return vm_ReadByte(address);
}

//Set the pc

void SetNextCode(vm_cpu* vm, uint16_t address) {
    #ifdef DEV_ERROR_CHECKING
    if (address > vm->codeSize)
        SYSTEM_ERROR(PROG_SEG_FAULT);
    #endif // ERROR_CHECKING_ENABLED
    vm->pc = address;
}

/*Returns 1 if the passed number of opCodes are available for contiguous 
 * reading from the current pc value. 0 if not.*/

uint8_t OpCodesAvailable(vm_cpu* vm, uint8_t opCodeCount) {
    if (vm->pc + opCodeCount <= vm->codeSize)
        return TRUE;
    else
        return FALSE;
}

//Reads and returns a 16bit value from program memory - Advances the pc.
uint16_t Read16bitOperand(vm_cpu* vm) {
    vm_value v;    
    #ifdef DEV_ERROR_CHECKING
    if (vm->pc+1 >= vm->codeSize)
        SYSTEM_ERROR(PROG_SEG_FAULT);
    #endif // ERROR_CHECKING_ENABLED  
    v.bytes.low = vm_ReadByte(vm->pc++);
    v.bytes.midLow = vm_ReadByte(vm->pc++);
    return v.uint16;
}
