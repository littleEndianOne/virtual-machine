#include "vm_printers.h"

void PrintOpCode(vm_opcode opcode) {
    switch (opcode) {
        case CONSTF:
            printf("%u (CONST)", opcode);
            break;
        case CONSTF8:
            printf("%u  (CONST8)", opcode);
            break;
        case CONSTF16:
            printf("%u (CONST16)", opcode);
            break;
        case CONSTFN0:
            printf("%u (CONSTN0)", opcode);
            break;
        case CONSTFN1:
            printf("%u (CONSTN1)", opcode);
            break;
        case CONSTFN2:
            printf("%u (CONSTN2)", opcode);
            break;
        case CONSTFN3:
            printf("%u (CONSTN3)", opcode);
            break;
        case CONSTFN4:
            printf("%u (CONSTN4)", opcode);
            break;
        case CONSTFN5:
            printf("%u (CONSTN5)", opcode);
            break;
        case CONSTFN6:
            printf("%u (CONSTN6)", opcode);
            break;
        case CONSTFN7:
            printf("%u (CONSTN7)", opcode);
            break;
        case CONSTFN8:
            printf("%u (CONSTN8)", opcode);
            break;
        case CONSTFN9:
            printf("%u (CONSTN9)", opcode);
            break;
        case CONSTFN10:
            printf("%u (CONSTN10)", opcode);
            break;
        case CONSTI:
            printf("%u (CONSTI)", opcode);
            break;
        case CONSTI8:
            printf("%u (CONSTI8)", opcode);
            break;
        case CONSTI16:
            printf("%u (CONSTI16)", opcode);
            break;
        case CONSTIN0:
            printf("%u (CONSTIN0)", opcode);
            break;
        case STRLIT:
            printf("%u (STRLIT)", opcode);
            break;
        case ADDF:
            printf("%u (ADD)", opcode);
            break;
        case SUBF:
            printf("%u (SUB)", opcode);
            break;
        case DIVF:
            printf("%u (DIV)", opcode);
            break;
        case MULF:
            printf("%u (MUL)", opcode);
            break;
        case EQF:
            printf("%u (EQ)", opcode);
            break;
        case LTF:
            printf("%u (LT)", opcode);
            break;
        case GTF:
            printf("%u (GT)", opcode);
            break;
        case NEQF:
            printf("%u (NEQ)", opcode);
            break;
        case ADDI:
            printf("%u (ADDI)", opcode);
            break;
        case SUBI:
            printf("%u (SUBI)", opcode);
            break;
        case DIVI:
            printf("%u (DIVI)", opcode);
            break;
        case MULI:
            printf("%u (MULI)", opcode);
            break;
        case EQI:
            printf("%u (EQI)", opcode);
            break;
        case LTI:
            printf("%u (LTI)", opcode);
            break;
        case GTI:
            printf("%u (GTI)", opcode);
            break;
        case NEQI:
            printf("%u (NEQI)", opcode);
            break;
        case JMP:
            printf("%u (JMP)", opcode);
            break;
        case JMPT:
            printf("%u (JMPT)", opcode);
            break;
        case JMPF:
            printf("%u (JMPF)", opcode);
            break;
        case CALL:
            printf("%u (CALL)", opcode);
            break;
        case RET:
            printf("%u (RET)", opcode);
            break;
        case LDARG:
            printf("%u (LDARG)", opcode);
            break;
        case DSTR:
            printf("%u (STR)", opcode);
            break;
        case STORE:
            printf("%u (STORE)", opcode);
            break;
        case LOAD:
            printf("%u (LOAD)", opcode);
            break;
        case APPND:
            printf("%u (APPND)", opcode);
            break;
        case TOSTR:
            printf("%u (CONV)", opcode);
            break;
        case GDSTR:
            printf("%u (GSTR)", opcode);
            break;
        case GSTORE:
            printf("%u (GSTORE)", opcode);
            break;
        case GLOAD:
            printf("%u (GLOAD)", opcode);
            break;
        case GAPPND:
            printf("%u (GAPPND)", opcode);
            break;
        case GCONV:
            printf("%u (GCONV)", opcode);
            break;
        case POP:
            printf("%u (POP)", opcode);
            break; 
        case DARRAY:
                printf("%u (DARAY)", opcode);
            break;
        case STOREAE:
            printf("%u (STOREAE)", opcode);
            break;
        case LOADAE:
            printf("%u (LOADAE)", opcode);
            break;
        case GDARRAY:
            printf("%u (GDARAY)", opcode);
            break;
        case GSTOREAE:
            printf("%u (GSTOREAE)", opcode);
            break;
        case GLOADAE:
            printf("%u (GLOADAE)", opcode);
            break;
        case HALT:
            printf("%u (HALT)", opcode);
            break;
        default:
            printf("%u (Invalid Opcode)", opcode);
            break;
    }
}

void PrintError(vm_error e) {
    switch (e) {
        case OK:
            printf("%u (OK)", e);
            break;
        case DIV0:
            printf("%u (Div0)", e);
            break;
        case INVALID_PARAM:
            printf("%u (Invalid Parameter)", e);
            break;
        case INVALID_STRING:
            printf("%u (Invalid String)", e);
            break;
        case MALLOC_FAULT:
            printf("%u (Malloc Fault)", e);
            break;
        case MEM_SEG_FAULT:
            printf("%u  (Memory Segmentation Fault)", e);
            break;
        case NOT_IMPLEMENTED:
            printf("%u (Opcode not implemented)", e);
            break;
        case PROG_SEG_FAULT:
            printf("%u (Program Memory Segmentation Fault)", e);
            break;
        case STACK_OVERFLOW:
            printf("%u (Op Stack Overflow)", e);
            break;
        case STACK_SEG_FAULT:
            printf("%u (Stack Segmentation Fault)", e);
            break;
        case STACK_UNDERRUN:
            printf("%u (Stack Underrun)", e);
            break;
        case TYPE_ERROR:
            printf("%u (Type Mismatch)", e);
            break;
        case TYPE_NOT_SET:
            printf("%u (Type Not Set)", e);
            break;
        case UNKNOWN_OPCODE:
            printf("%u (Unknown Opcode)", e);
            break;
        case TYPE_UNKNOWN:
            printf("%u (Unknown Type)", e);
            break;
        case RET_NO_SCOPE:
            printf("%u (Return opcode without function scope)", e);
            break;
        case UNKNOWN_ERROR:
            printf("%u (Unknown Error)", e);
            break;
        default:
            printf("%u (Error unknown to printer)", e);
            break;
    }
}

void PrintElement(vm_element* e) {
    switch (e->type) {
        case NONE:
            printf("NONE    | %#x", e->value.uint32);
            break;
        case FLOAT:
            printf("FLOAT     | %f", e->value.float32);
            break;
        case INTEGER:
            printf("INTEGER     | %i", e->value.int32);
            break;
        case STRING_LIT:
            /*string lit is uint16 but printf promotes to 4B anyway so 
             * using uint32 avoids warning.*/
            printf("STRING_LIT | %u", e->value.strLit.address);
            break;
        case STRING_REF:
            printf("STRING_REF | 0x%p", e->value.memPtr);
            break;
        case ARRAY_REF:
            printf("ARRAY_REF | 0x%p", e->value.memPtr);
            break;
        default:
            printf("UNKNOWN    | %#x", e->value.uint32);
            break;
    }
}

void PrintRunState(vm_state runState) {
    switch (runState) {
        case READY:
            printf("Ready");
            break;
        case RUNNING_EVENT_HANDLER:
            printf("Running");
            break;
        case HALTED:
            printf("Halted");
            break;
        case FAULT:
            printf("FAULT");
            break;
        default:
            printf("Unknown to printer");
            break;
    }
}

void PrintCallStack(vm_scope* s) {
    printf("Call Stack\n");
    printf("------------------------------------\n");
    PrintScope(s);
}

/*
Loop through the contents of the operand stack and use the element type to format the output.
 */
void PrintOpStack(vm_element* opStack, int16_t sp) {

    int i = sp;
    printf("Op Stack\n");
    printf("-------------------------------------\n");

    if (i == -1)
        printf("-Empty- \n");

    for (; i>-1; i--) {
        vm_element* operand = &opStack[i];
        printf("%4i | ", i);

        PrintElement(operand);

        printf("\n");
    }
}

void PrintScope(vm_scope* s) {
    if (s != NULL) {
        printf("Stack FP: %u \n", s->stackFP);
        printf("Return PC: %u \n", s->returnPC);
        printf("Locals Count: %u \n", s->localsCount);
        printf("Arg Count: %u \n", s->argCount);

        printf("Locals: \n");
        PrintVarArray(s->localsArray, s->localsCount);

        if (s->prevScope != NULL) {
            printf("----------------------\n");
            PrintScope(s->prevScope);
        }
    }
    else {
        printf("-Empty-\n");
    }
}

void PrintVarArray(vm_element* array, uint8_t count) {
    if (count == 0) {
        printf("-None-\n");
    }
    else {
        uint8_t i;
        for (i = 0; i < count; i++) {
            PrintElement(&(array[i]));
            printf("\n");
        }
    }
}

void PrintThreadState(vm_cpu* vm) {
    printf("VM State\n");
    printf("-------------------------------------------------\n");
    printf("Error State: ");
    PrintError(vm->errorCode);
    printf("\nRunState: ");
    PrintRunState(vm->runState);
    printf("\n");
    printf("Program Counter: %u \n", vm->pc);
    printf("Code Size: %u \n", vm->codeSize);
    printf("Stack Pointer: %i \n", vm->sp);
    printf("Current Opcode: ");
    PrintOpCode(vm->opcode);
    printf("\n");
    printf("Globals Count: %u \n", vm->globalsCount);
    //Globals dump...
    printf("Globals: \n");
    PrintVarArray(vm->globalsArray, vm->globalsCount);
    printf("\n");
    PrintOpStack(vm->opStack, vm->sp);
    printf("\n");
    PrintCallStack(vm->localScope);
    printf("\n");
}



