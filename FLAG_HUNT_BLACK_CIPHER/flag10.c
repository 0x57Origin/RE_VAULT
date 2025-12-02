#include "flaghunt2.h"

uint32_t custom_md5_variant(const char* input) {
    uint32_t hash = 0x67452301;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        hash += (unsigned char)input[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    
    return hash;
}

uint32_t custom_sha1_variant(const char* input) {
    uint32_t hash = 0xEFCDAB89;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        hash ^= (unsigned char)input[i];
        hash *= 0x01000193;
        hash = ((hash << 5) | (hash >> 27));
    }
    
    return hash;
}

uint32_t fnv1_variant(const char* input) {
    uint32_t hash = 0x811C9DC5;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        hash ^= (unsigned char)input[i];
        hash *= 0x01000193;
        hash ^= 0xAAAAAAAA;
    }
    
    return hash;
}

uint32_t rotate_xor_hash(const char* input) {
    uint32_t hash = 0x55555555;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 7) | (hash >> 25));
        hash ^= (unsigned char)input[i];
        hash += 0x9E3779B9;
    }
    
    return hash;
}

uint32_t folding_checksum(const char* input) {
    uint32_t hash = 0;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i += 4) {
        uint32_t chunk = 0;
        for (int j = 0; j < 4 && (i + j) < len; j++) {
            chunk |= ((uint32_t)(unsigned char)input[i + j]) << (j * 8);
        }
        hash += chunk;
        hash = (hash & 0xFFFF) + (hash >> 16);
    }
    
    return hash;
}

static uint32_t apply_hash_chain(const char* input) {
    uint32_t stage1 = custom_md5_variant(input);
    
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "%08X", stage1);
    uint32_t stage2 = custom_sha1_variant(temp_str);
    
    snprintf(temp_str, sizeof(temp_str), "%08X", stage2);
    uint32_t stage3 = fnv1_variant(temp_str);
    
    snprintf(temp_str, sizeof(temp_str), "%08X", stage3);
    uint32_t stage4 = rotate_xor_hash(temp_str);
    
    snprintf(temp_str, sizeof(temp_str), "%08X", stage4);
    uint32_t stage5 = folding_checksum(temp_str);
    
    return stage5;
}

int multi_stage_hash(const char* input) {
    if (!input) {
        return 0;
    }
    
    uint32_t final_hash = apply_hash_chain(input);
    
    if (final_hash == 0x00000B73) {
        return 1;
    }
    
    return 0;
}