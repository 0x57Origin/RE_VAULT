#include "flaghunt2.h"

#define OP_NOP    0x00
#define OP_PUSH   0x01
#define OP_POP    0x02
#define OP_ADD    0x03
#define OP_SUB    0x04
#define OP_MUL    0x05
#define OP_DIV    0x06
#define OP_XOR    0x07
#define OP_AND    0x08
#define OP_OR     0x09
#define OP_LOAD   0x0A
#define OP_STORE  0x0B
#define OP_JMP    0x0C
#define OP_JZ     0x0D
#define OP_CMP    0x0E
#define OP_PRINT  0x0F
#define OP_ROT    0x10
#define OP_HALT   0x11

const vm_instruction_t bytecode_program[] = {
    {OP_PUSH, 70},    // 'F'
    {OP_PRINT, 0},
    {OP_PUSH, 72},    // 'H'
    {OP_PRINT, 0},
    {OP_PUSH, 50},    // '2'
    {OP_PRINT, 0},
    {OP_PUSH, 95},    // '_'
    {OP_PRINT, 0},
    {OP_PUSH, 70},    // 'F'
    {OP_PRINT, 0},
    {OP_PUSH, 76},    // 'L'
    {OP_PRINT, 0},
    {OP_PUSH, 65},    // 'A'
    {OP_PRINT, 0},
    {OP_PUSH, 71},    // 'G'
    {OP_PRINT, 0},
    {OP_PUSH, 95},    // '_'
    {OP_PRINT, 0},
    {OP_PUSH, 83},    // 'S'
    {OP_PRINT, 0},
    {OP_PUSH, 73},    // 'I'
    {OP_PRINT, 0},
    {OP_PUSH, 88},    // 'X'
    {OP_PRINT, 0},
    {OP_PUSH, 95},    // '_'
    {OP_PRINT, 0},
    {OP_PUSH, 50},    // '2'
    {OP_PRINT, 0},
    {OP_PUSH, 55},    // '7'
    {OP_PRINT, 0},
    {OP_PUSH, 51},    // '3'
    {OP_PRINT, 0},
    {OP_PUSH, 52},    // '4'
    {OP_PRINT, 0},
    {OP_PUSH, 10},    // '\n'
    {OP_PRINT, 0},
    {OP_HALT, 0}
};

const vm_instruction_t decoy_bytecode_1[] = {
    {OP_PUSH, 65},
    {OP_PUSH, 66},
    {OP_ADD, 0},
    {OP_POP, 0},
    {OP_HALT, 0}
};

const vm_instruction_t decoy_bytecode_2[] = {
    {OP_PUSH, 100},
    {OP_PUSH, 50},
    {OP_MUL, 0},
    {OP_STORE, 0},
    {OP_HALT, 0}
};

static void vm_execute_instruction(vm_state_t* vm, const vm_instruction_t* instr) {
    switch (instr->opcode) {
        case OP_NOP:
            break;
            
        case OP_PUSH:
            if (vm->sp < VM_STACK_SIZE) {
                vm->stack[vm->sp++] = instr->operand;
            }
            break;
            
        case OP_POP:
            if (vm->sp > 0) {
                vm->sp--;
            }
            break;
            
        case OP_ADD:
            if (vm->sp >= 2) {
                uint32_t b = vm->stack[--vm->sp];
                uint32_t a = vm->stack[--vm->sp];
                vm->stack[vm->sp++] = a + b;
            }
            break;
            
        case OP_SUB:
            if (vm->sp >= 2) {
                uint32_t b = vm->stack[--vm->sp];
                uint32_t a = vm->stack[--vm->sp];
                vm->stack[vm->sp++] = a - b;
            }
            break;
            
        case OP_MUL:
            if (vm->sp >= 2) {
                uint32_t b = vm->stack[--vm->sp];
                uint32_t a = vm->stack[--vm->sp];
                vm->stack[vm->sp++] = a * b;
            }
            break;
            
        case OP_XOR:
            if (vm->sp >= 2) {
                uint32_t b = vm->stack[--vm->sp];
                uint32_t a = vm->stack[--vm->sp];
                vm->stack[vm->sp++] = a ^ b;
            }
            break;
            
        case OP_PRINT:
            if (vm->sp > 0) {
                printf("%c", (char)vm->stack[vm->sp - 1]);
                vm->sp--;
            }
            break;
            
        case OP_HALT:
            vm->flags |= 1;
            break;
            
        default:
            break;
    }
}

void run_vm(void) {
    vm_state_t vm = {0};
    
    while (!(vm.flags & 1) && vm.pc < (sizeof(bytecode_program) / sizeof(vm_instruction_t))) {
        vm_execute_instruction(&vm, &bytecode_program[vm.pc]);
        vm.pc++;
    }
}