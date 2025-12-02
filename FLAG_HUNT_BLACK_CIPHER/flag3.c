#include "flaghunt2.h"

static uint32_t rotate_left(uint32_t value, int shift) {
    return (value << shift) | (value >> (32 - shift));
}

int polynomial_hash_check(const char* input) {
    if (!input) return 0;
    
    uint32_t hash = 0;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        uint32_t char_val = (uint32_t)(unsigned char)input[i];
        uint32_t multiplied = char_val * 1315423911;
        int rotation = i % 5;
        hash ^= rotate_left(multiplied, rotation);
    }
    
    if (hash == 0x8B3A9F72) {
        return 1;
    }
    
    return 0;
}

void fake_polynomial_1(const char* input) {
    if (!input) return;
    
    uint32_t hash = 0;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        hash += input[i] * 97;
        hash = rotate_left(hash, 3);
    }
    
    if (hash == 0xDEADBEEF) {
        printf("Fake path activated\n");
    }
}

void fake_polynomial_2(const char* input) {
    if (!input) return;
    
    uint32_t hash = 1;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        hash *= input[i];
        hash ^= 0xCAFEBABE;
    }
    
    if (hash == 0x12345678) {
        printf("Alternative path found\n");
    }
}