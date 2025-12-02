#ifndef FLAGHUNT2_H
#define FLAGHUNT2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <time.h>

#define MAX_INPUT 256
#define VM_STACK_SIZE 128
#define NUM_OPCODES 18

typedef struct {
    uint8_t opcode;
    uint32_t operand;
} vm_instruction_t;

typedef struct {
    uint32_t stack[VM_STACK_SIZE];
    uint32_t sp;
    uint32_t pc;
    uint8_t registers[8];
    uint32_t flags;
} vm_state_t;

typedef struct {
    char marker[8];
    uint32_t checksum;
    char encoded_data[32];
    uint32_t size;
} heap_artifact_t;

typedef void (*func_ptr_t)(int);

extern const unsigned char hidden_data_section[];
extern const vm_instruction_t bytecode_program[];
extern const unsigned char fake_pcap_data[];
extern const uint32_t ghost_fragment_1;
extern const uint16_t ghost_fragment_2;
extern const uint8_t ghost_fragment_3;

char* decrypt_string(const unsigned char* encrypted, size_t len, uint8_t key);
void function_maze_entry(int choice);
int polynomial_hash_check(const char* input);
void extract_hidden_flag(void);
int anti_debug_check(void);
void run_vm(void);
int math_logic_challenge(int x, int y, int z);
void heap_challenge(void);
int pcap_decoder_challenge(const char* protocol_input);
int multi_stage_hash(const char* input);

void dead_function_1(int x);
void dead_function_2(int x);
void dead_function_3(int x);
void dead_function_4(int x);
void flag_printer_maze_1(int x);
void flag_printer_maze_2(int x);
void flag_printer_maze_3(int x);
void flag_printer_maze_4(int x);
void flag_printer_maze_5(int x);
void redirect_function_1(int x);
void redirect_function_2(int x);

uint32_t custom_md5_variant(const char* input);
uint32_t custom_sha1_variant(const char* input);
uint32_t fnv1_variant(const char* input);
uint32_t rotate_xor_hash(const char* input);
uint32_t folding_checksum(const char* input);

void print_menu(void);
int get_user_choice(void);
void handle_challenge(int choice);

#endif