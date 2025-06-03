#pragma once

#include "../VM/vm_common.h"

/*Function that must be implemented to interface with whatever memory is used to
 * store the program code.
 */
uint8_t vm_ReadByte(uint16_t address);

/*
 * The implementation of the progmem accessing functions must implement the 
 * following functions. 
 */

/* Get next opcode and advance pc.
 * Extern inline declaration to provide symbol for non in-lined calls.
 * See header file for more details.
 */
uint8_t NextCode(vm_cpu* vm);

// Get next opcode without advancing PC.
int8_t PeakNextCode(vm_cpu* vm);

//Return the opcode at the passed address.
uint8_t PeakCodeAt(vm_cpu* vm, uint16_t codeAddress);

//Set the PC to the passed address
void SetNextCode(vm_cpu* vm, uint16_t address);


//Returns TRUE if the passed number of opCodes are available for contiguous reading from the current PC
//value. FALSE if not.
boolean_t OpCodesAvailable(vm_cpu* vm, uint8_t opCodeCount);

//Read and return a 16bit value.
uint16_t Read16bitOperand(vm_cpu* vm);

//Read and return a 32bit value.
//uint32_t Get32bitOperand(vm_thread* vm);


/*A C99 model. Use inline in a common header, and provide definitions in a .c file somewhere, via extern declarations. For instance, in the header file :
 * inline int max(int a, int b) {
 *       return a > b ? a : b;
 *}
 * ...and in exactly one source file :
 *
 * #include "header.h"
 * extern int max(int a, int b);
 *
 * Explanation:
 * A function where at least one declaration mentions inline, but where some declaration doesn't mention inline or does mention extern. 
 * There must be a definition in the same translation unit. 
 * Stand-alone object code is emitted (just like a normal function) and can be called from other translation units in your program.
 * The same constraint about statics above applies here, too.
 * In this example all the declarations and definitions use inline but one adds extern:

 * // a declaration mentioning extern and inline
 * extern inline int max(int a, int b);
 * 
 * // a definition mentioning inline
 * inline int max(int a, int b) {
 *  return a > b ? a : b;
 * }
 * 
 * In this example, one of the declarations does not mention inline:
 * // a declaration not mentioning inline
 * int max(int a, int b);
 * // a definition mentioning inline
 * inline int max(int a, int b) {
 * return a > b ? a : b;
 * }
 * In either example, the function will be callable from other files.
 */
