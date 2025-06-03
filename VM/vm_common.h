#pragma once

#include <setjmp.h>
#include <stdint-gcc.h>


#define TRUE 1
#define FALSE 0

typedef uint8_t boolean_t;

//Define to enable machine error checking that stops the program and sets error states.
#define DEV_ERROR_CHECKING

//Run state of the VM.
typedef enum VMRunState {
    READY = 0, //initialized and ready to run
    WAITING_FOR_EVENT = 1, //Running event loop
    RUNNING_EVENT_HANDLER = 2, //executing opcodes
    FAULT = 3, //terminated due to system fault
    HALTED = 4 //terminated without error by HAULT opcode
} vm_state;

//Operators



//The type of elements that can be on the operand stack and in variables.

typedef enum Types {
    NONE = 0, //No type set - used as initial value    
    INTEGER = 1,
    FLOAT = 2, //A floating point number (float32)
    STRING_LIT = 3, //Reference to string stored in prog mem
    STRING_REF = 4, //Reference to string stored in local or global variable
    ARRAY_REF = 5, //Reference to a NUMBER array in a local or global variable            
} vm_type;

//String stored in a local or global variable

typedef struct StringRef {
    uint8_t allocLength; //Length of the memory allocation for storing chars
    char* charPtr; //Pointer to first char of the string
} vm_strref;

//String in prog-mem

typedef struct StringLit {
    uint8_t length; //Length of the string
    uint16_t address; //Prog mem start address of the string
} vm_strlit;

typedef struct Bytes {
    uint8_t low;
    uint8_t midLow;
    uint8_t midHigh;
    uint8_t high;
} vm_bytes;

typedef union TypesUnion {
    struct Bytes bytes;
    uint32_t uint32;
    uint16_t uint16;
    uint8_t uint8;
    int32_t int32;
    int16_t int16;
    float float32;
    vm_strref strRef;
    vm_strlit strLit;
    struct Element* memPtr;
    struct Array* arrayPtr;
} vm_value;

//Stores type and data for stack elements and variables.

typedef struct Element {
    enum Types type;
    union TypesUnion value;
} vm_element;

//Struct for managing an array in a variable.

typedef struct Array {
    uint8_t size;
    vm_element* data;
} vm_array;

//System errors thrown by VM when a fault occurs.

typedef enum VMError {
    OK = 0, // NONE must be zero for setjmp to work.
    DIV0 = 1, // Divide by 0 attempted
    PROG_SEG_FAULT = 2, // Attempt to access code address outside addressable program memory
    STACK_OVERFLOW = 3, // Operand stack is full
    STACK_UNDERRUN = 4, // Attempt to pop from empty operand stack
    STACK_SEG_FAULT = 5, // Attempt to access stack element outside of addressable stack memory 
    UNKNOWN_OPCODE = 6, // Attempted execution of unknown opcode value
    TYPE_ERROR = 7, // Attempted operation on incorrect type. e.g. Global/Local store or conversion
    TYPE_NOT_SET = 8, // Attempted to load from from variable address that has not been stored to.
    TYPE_UNKNOWN = 9, // Invalid/unexpected type value encountered
    MEM_SEG_FAULT = 10, // Attempt to access local or global variable address outside of the memory allocation.
    MALLOC_FAULT = 11, // Memory allocation failed
    INVALID_STRING = 12, // String not constructed properly.
    INVALID_PARAM = 13, // An op code has been followed by an invalid parameter for that operator.
    RET_NO_SCOPE = 14,
    NOT_IMPLEMENTED = 254, // Attempt to use a feature with no implementation
    UNKNOWN_ERROR = 255 // An unidentified error code.
} vm_error;

//Stores the data for a running function.

typedef struct LocalScope {
    uint16_t returnPC; //PC value for function return.
    int16_t stackFP; //SP for beginning of a function frame on the operand stack
    uint8_t argCount; //Number of Arguments passed to function
    uint8_t localsCount; //Number of local variable address created for function scope   
    struct Element* localsArray; //Pointer to local variables storage array
    struct LocalScope* prevScope; //Pointer to previous/parent functions scope. Used for returning to the previous scope on return.
} vm_scope;

typedef enum Opcode {
    CONSTF = 1, //Push 32b float
    CONSTF8 = 2, //Push 32b float from 8b singed -128 to 127
    CONSTF16 = 3, //Push 32b float from 16b integer
    CONSTFN0 = 4, //Push float with value 0
    CONSTFN1 = 5, //Push float with value 1
    CONSTFN2 = 6, //Push float with value 2
    CONSTFN3 = 7, //Push float with value 3
    CONSTFN4 = 8, //Push float with value 4
    CONSTFN5 = 9, //Push float with value 5
    CONSTFN6 = 10, //Push float with value 6
    CONSTFN7 = 11, //Push float with value 7
    CONSTFN8 = 12, //Push float with value 8
    CONSTFN9 = 13, //Push float with value 9
    CONSTFN10 = 14, //Push float with value 10
    CONSTI = 15, //Push 32b signed int
    CONSTI8 = 16, //8b singed int -128 to 127
    CONSTI16 = 17, //Push 16b int
    CONSTIN0 = 18, //Push int with value 0
    STRLIT = 19, //Load code address for string literal onto operand stack.
    ADDF = 20, //Add two number types
    SUBF = 21, //Subtract a number from another
    DIVF = 22, //Divide a number by another
    MULF = 23, //Multiply a number by another
    EQF = 24, //Number equal a == b : 1 = True 0 = False
    LTF = 25, //Number less than a < b : 1 = True 0 = False
    GTF = 26, //Number greater than a > b : 1 = True 0 = False
    NEQF = 27, //Number not equal a!=b : 1 = True 0 = False
    ADDI = 28, //Add two number types
    SUBI = 29, //Subtract a number from another
    DIVI = 30, //Divide a number by another
    MULI = 31, //Multiply a number by another
    EQI = 32, //Number equal a == b : 1 = True 0 = False
    LTI = 33, //Number less than a < b : 1 = True 0 = False
    GTI = 34, //Number greater than a > b : 1 = True 0 = False
    NEQI = 35, //Number not equal a!=b : 1 = True 0 = False
    JMP = 36, //Unconditional jump to prog mem address
    JMPT = 37, //Jump to prog mem address if top stack value is 1
    JMPF = 38, //Jump to prog mem address if top stack value is 0
    CALL = 39, //Call function at address
    RET = 40, //Return from function
    LDARG = 41, //Load function arguments at address
    DSTR = 42, //Create a string of 'size' at local 'address'
    STORE = 43, //Store top stack element in local variable at address
    LOAD = 44, //Load stack element from local variable at address
    APPND = 45, //Append number, strRef, strLit type to string at local 'address'
    TOSTR = 46, //Convert a number (float/integer) type to a string with 'precision'. Pops: Number.
    GDSTR = 47, //Create a string of 'size' at global 'address'
    GSTORE = 48, //Store top stack element in global variable at address
    GLOAD = 49, //Load stack element from global variable at address
    GAPPND = 50, //Append number, strRef, strLit type to string at global address
    GCONV = 51, //Convert a number type to a string with 'precision' and store it in global variable at 'address'
    POP = 52, //Throw away element on top of the stack
    DARRAY = 53, //Create a locally stored array at address.
    STOREAE = 54, //Store an array element locally at index.
    LOADAE = 55, //Load a locally stored array element onto the stack.
    GDARRAY = 56, //Create a globally stored array at address.
    GSTOREAE = 57, //Store an array element globally at index.
    GLOADAE = 58, //Load a global store array element onto the stack.
    PROMOTE = 59, //Promote an integer into a float (Round to nearest integer).
    DEMOTE = 60, //Demote a float into an integer.
    END = 61, //End execution of an event handler.
	SET_EVENT_HANDLER = 62, //Set the progmem handler address for an event.
    HALT = 255, //Stop vm execution
} vm_opcode;

typedef struct vm_EventBuffer_Event {
	int8_t event_id;
    vm_element param;
} vm_eventBuffer_event;

typedef struct vm_EventBuffer {
	uint8_t capacity; //Used to check for a full buffer.
	uint8_t mask; //Used to map index values into the array.

	struct vm_EventBuffer_Event* array;
	uint8_t read; //Index of next event to be read from buffer.
	uint8_t write; //Index of position that next event will be written to.

} vm_eventBuffer;

//Stores the data for a VM thread.
typedef struct vm_CPU{
    uint8_t globalsCount; //Globally scoped string variables
    vm_element* globalsArray; //Globally scoped number variables
    vm_element* opStack; //Virtual stack
    uint16_t stackSize;
    uint16_t pc; //Program counter
    int16_t sp; //Stack pointer -ve 1 indicates empty stack (needs to be signed)
    vm_scope* localScope; //Pointer to current function scope struct (for local scope)
    vm_scope* initScope; //Pointer to initial (default) local scope. Used for checking on RET.
    vm_opcode opcode; //Current executing opcode
    vm_state runState; //Current run state of VM
    vm_error errorCode; //System error code that caused FAULT state.
    jmp_buf sysErrorEnv; //Reset point buffer for system errors.
    uint16_t codeSize; //The byte count of the current program.
    struct vm_EventBuffer* eventBuffer;
    uint16_t* eventHandlers;
    uint8_t eventBufferSize;
    uint8_t eventHandlerCount;
    void (*systemError)(struct vm_CPU*);
    void (*eventBufferAccessStart)(struct vm_CPU*);
    void (*eventBufferAccessFinish)(struct vm_CPU*);
    void (*eventBufferEmpty)(struct vm_CPU*);
} vm_cpu;


//Macro for halting the VM using a longjmp call.
#define SYSTEM_ERROR(errorCode) (longjmp(vm->sysErrorEnv, errorCode))
