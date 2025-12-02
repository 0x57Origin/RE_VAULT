#include "flaghunt2.h"

void heap_challenge(void) {
    heap_artifact_t* artifact = (heap_artifact_t*)malloc(sizeof(heap_artifact_t));
    
    if (!artifact) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    strcpy(artifact->marker, "HEAP001");
    artifact->checksum = 0xDEADBEEF;
    artifact->size = 19;
    
    unsigned char encoded[] = {
        0x83, 0x87, 0x77, 0x50, 0x81, 0x87, 0x84, 0x8a,
        0x50, 0x86, 0x8c, 0x8a, 0x87, 0x50, 0x74, 0x75,
        0x74, 0x75, 0x00
    };
    
    memcpy(artifact->encoded_data, encoded, sizeof(encoded));
    
    volatile char* ptr_leak = (char*)artifact;
    for (int i = 0; i < 1000; i++) {
        ptr_leak += (i % 2) ? 1 : -1;
    }
    
    printf("Heap structure analyzed. Artifact located at: %p\n", (void*)artifact);
    
    char decoded[32];
    for (size_t i = 0; i < artifact->size; i++) {
        decoded[i] = artifact->encoded_data[i] ^ 0x55;
    }
    decoded[artifact->size] = '\0';
    
    printf("%s\n", decoded);
    
    char* fake_data = (char*)malloc(256);
    if (fake_data) {
        memset(fake_data, 0xAA, 256);
        strcpy(fake_data, "FAKE_HEAP_DATA_IGNORE_THIS");
        free(fake_data);
    }
    
    free(artifact);
}