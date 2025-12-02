#include "flaghunt2.h"

const unsigned char fake_pcap_data[] = {
    // Fake PCAP header
    0xD4, 0xC3, 0xB2, 0xA1, 0x02, 0x00, 0x04, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    
    // Fake packet header
    0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
    0x2A, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x00, 0x00,
    
    // Fake ethernet header
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0x08, 0x00,
    
    // Encrypted flag data (XOR with 0x3A)
    0x7C, 0x7E, 0x68, 0x65, 0x7C, 0x66, 0x7E, 0x71,
    0x65, 0x75, 0x7F, 0x75, 0x71, 0x65, 0x69, 0x6D,
    0x6B, 0x6B, 0x0A
};

static int validate_protocol_tcp(const char* input) {
    return strcmp(input, "tcp") == 0;
}

static int validate_protocol_udp(const char* input) {
    return strcmp(input, "udp") == 0;
}

static int validate_protocol_icmp(const char* input) {
    return strcmp(input, "icmp") == 0;
}

static int validate_protocol_http(const char* input) {
    return strcmp(input, "http") == 0;
}

static int validate_packet_structure(void) {
    if (fake_pcap_data[0] == 0xD4 && fake_pcap_data[1] == 0xC3) {
        return 1;
    }
    return 0;
}

static int extract_payload(char* output, size_t max_len) {
    const unsigned char* payload_start = fake_pcap_data + 40;
    size_t payload_len = 19;
    
    if (payload_len >= max_len) {
        return 0;
    }
    
    for (size_t i = 0; i < payload_len; i++) {
        output[i] = payload_start[i] ^ 0x3A;
    }
    output[payload_len] = '\0';
    
    return 1;
}

int pcap_decoder_challenge(const char* protocol_input) {
    if (!protocol_input) {
        return 0;
    }
    
    int protocol_valid = 0;
    
    if (validate_protocol_tcp(protocol_input)) {
        protocol_valid = 1;
    } else if (validate_protocol_udp(protocol_input)) {
        printf("UDP protocol detected, but payload encrypted.\n");
        return 0;
    } else if (validate_protocol_icmp(protocol_input)) {
        printf("ICMP protocol detected, no payload.\n");
        return 0;
    } else if (validate_protocol_http(protocol_input)) {
        protocol_valid = 1;
    } else {
        printf("Unknown protocol: %s\n", protocol_input);
        return 0;
    }
    
    if (!protocol_valid) {
        return 0;
    }
    
    if (!validate_packet_structure()) {
        printf("Invalid packet structure.\n");
        return 0;
    }
    
    char decoded_payload[64];
    if (!extract_payload(decoded_payload, sizeof(decoded_payload))) {
        printf("Payload extraction failed.\n");
        return 0;
    }
    
    printf("Decoded payload: %s\n", decoded_payload);
    return 1;
}