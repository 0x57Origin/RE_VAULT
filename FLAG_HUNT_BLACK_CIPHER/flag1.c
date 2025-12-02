#include "flaghunt2.h"

const unsigned char encrypted_strings[] = {
    0x25, 0x29, 0x53, 0x74, 0x2c, 0x2e, 0x2c, 0x78,  // "FH2_FLAG"
    0x74, 0x20, 0x24, 0x22, 0x74, 0x58, 0x5a, 0x57,  // "_ONE_749"
    0x53                                               // "2"
};

const unsigned char decoy_strings[] = {
    0x33, 0x41, 0x55, 0x66, 0x34, 0x3e, 0x3c, 0x60,
    0x66, 0x32, 0x36, 0x30, 0x66, 0x40, 0x42, 0x45,
    0x55, 0x00
};

char* decrypt_string(const unsigned char* encrypted, size_t len, uint8_t key) {
    static char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    
    for (size_t i = 0; i < len && i < 63; i++) {
        uint8_t byte = encrypted[i];
        byte ^= key;
        byte = ((byte << 3) | (byte >> 5)) & 0xFF;
        byte += (i * 7) % 256;
        buffer[i] = byte;
    }
    
    return buffer;
}

void fake_decrypt_1(void) {
    decrypt_string(decoy_strings, sizeof(decoy_strings), 0x42);
}

void fake_decrypt_2(void) {
    decrypt_string(decoy_strings, sizeof(decoy_strings), 0x13);
}

void extract_hidden_flag(void) {
    fake_decrypt_1();
    
    char* result = decrypt_string(encrypted_strings, sizeof(encrypted_strings), 0x55);
    
    fake_decrypt_2();
    
    if (result && strlen(result) > 10) {
        printf("%s\n", result);
    } else {
        printf("String decryption failed.\n");
    }
}