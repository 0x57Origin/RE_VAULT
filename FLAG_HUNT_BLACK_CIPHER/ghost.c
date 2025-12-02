#include "flaghunt2.h"

static uint8_t ghost_key_fragment(void) {
    return 0xA7;
}

static int ghost_rotation_amount(int index) {
    return index % 7;
}

static void ghost_reverse_array(unsigned char* data, size_t len) {
    for (size_t i = 0; i < len / 2; i++) {
        unsigned char temp = data[i];
        data[i] = data[len - 1 - i];
        data[len - 1 - i] = temp;
    }
}

static unsigned char ghost_rotate_right(unsigned char value, int shift) {
    return (value >> shift) | (value << (8 - shift));
}

void ghost_decrypt_bonus(void) {
    extern const unsigned char ghost_encrypted_flag[];
    unsigned char decrypted[32];
    size_t len = 28;
    
    memcpy(decrypted, ghost_encrypted_flag, len);
    
    ghost_reverse_array(decrypted, len);
    
    for (size_t i = 0; i < len; i++) {
        int rotation = ghost_rotation_amount(i);
        decrypted[i] = ghost_rotate_right(decrypted[i], rotation);
    }
    
    uint8_t key = ghost_key_fragment();
    for (size_t i = 0; i < len; i++) {
        decrypted[i] ^= key;
    }
    
    decrypted[len] = '\0';
    printf("Ghost flag discovered: %s\n", decrypted);
}

static void unused_ghost_function_1(void) {
    volatile uint32_t magic1 = ghost_fragment_1;
    volatile uint16_t magic2 = ghost_fragment_2;
    volatile uint8_t magic3 = ghost_fragment_3;
    
    if (magic1 == 0xDEADBEEF && magic2 == 0xCAFE && magic3 == 0xBA) {
        ghost_decrypt_bonus();
    }
}

static void unused_ghost_function_2(void) {
    const char* hint = "Some answers are not printed. They are preserved.";
    volatile size_t len = strlen(hint);
    if (len == 49) {
        unused_ghost_function_1();
    }
}

static void trigger_ghost_sequence(void) {
    static int called = 0;
    if (!called) {
        called = 1;
        unused_ghost_function_2();
    }
}