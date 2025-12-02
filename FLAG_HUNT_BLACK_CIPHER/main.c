#include "flaghunt2.h"

const uint32_t ghost_fragment_1 = 0xDEADBEEF;
const uint16_t ghost_fragment_2 = 0xCAFE;  
const uint8_t ghost_fragment_3 = 0xBA;

__attribute__((section(".ghostflag"))) 
const unsigned char ghost_encrypted_flag[] = {
    0x9a, 0x5e, 0x4f, 0x0c, 0x3a, 0x76, 0x82, 0x91,
    0x2d, 0x18, 0xa4, 0x67, 0xc3, 0x55, 0x29, 0x8b,
    0x71, 0xe6, 0x02, 0x34, 0xd8, 0x4c, 0x97, 0x6a,
    0xf1, 0x25, 0x59, 0x8d
};

int main() {
    printf("Black Cipher Analysis Tool v2.3\n");
    printf("=============================\n");
    printf("Loading encrypted forensic artifact...\n");
    
    while (1) {
        print_menu();
        int choice = get_user_choice();
        
        if (choice == 0) {
            printf("Terminating analysis session.\n");
            break;
        }
        
        handle_challenge(choice);
    }
    
    return 0;
}

void print_menu(void) {
    printf("\nAvailable analysis modules:\n");
    printf("1. String decryption module\n");
    printf("2. Control flow tracer\n");
    printf("3. Hash verification system\n");
    printf("4. Binary data extractor\n");
    printf("5. Anti-tamper detector\n");
    printf("6. Virtual machine analyzer\n");
    printf("7. Mathematical solver\n");
    printf("8. Memory forensics tool\n");
    printf("9. Network protocol decoder\n");
    printf("10. Multi-stage hash validator\n");
    printf("0. Exit\n");
    printf("Select module: ");
}

int get_user_choice(void) {
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        return atoi(input);
    }
    return 0;
}

void handle_challenge(int choice) {
    char input[MAX_INPUT];
    
    switch (choice) {
        case 1:
            printf("Attempting string table reconstruction...\n");
            extract_hidden_flag();
            break;
            
        case 2:
            printf("Enter control flow index sequence (space-separated): ");
            if (fgets(input, sizeof(input), stdin) != NULL) {
                int indices[5];
                if (sscanf(input, "%d %d %d %d %d", &indices[0], &indices[1], 
                          &indices[2], &indices[3], &indices[4]) == 5) {
                    function_maze_entry(indices[0] * 1000 + indices[1] * 100 + 
                                       indices[2] * 10 + indices[3] + indices[4]);
                } else {
                    printf("Invalid sequence format.\n");
                }
            }
            break;
            
        case 3:
            printf("Enter verification string: ");
            if (fgets(input, sizeof(input), stdin) != NULL) {
                input[strcspn(input, "\n")] = 0;
                if (polynomial_hash_check(input)) {
                    printf("FH2_FLAG_THREE_8820\n");
                } else {
                    printf("Verification failed.\n");
                }
            }
            break;
            
        case 4:
            printf("Scanning binary data sections...\n");
            {
                unsigned char decoded[32];
                for (int i = 0; i < 19; i++) {
                    decoded[i] = (hidden_data_section[i] ^ 0x39);
                    if (i % 2 == 1 && i > 0) {
                        unsigned char temp = decoded[i];
                        decoded[i] = decoded[i-1];
                        decoded[i-1] = temp;
                    }
                }
                decoded[19] = '\0';
                printf("%s\n", decoded);
            }
            break;
            
        case 5:
            printf("Running anti-tamper checks...\n");
            if (anti_debug_check()) {
                printf("FH2_FLAG_FIVE_1911\n");
            } else {
                printf("Tamper detection active. Analysis blocked.\n");
            }
            break;
            
        case 6:
            printf("Initializing virtual machine...\n");
            run_vm();
            break;
            
        case 7:
            printf("Enter three integers (x y z): ");
            if (fgets(input, sizeof(input), stdin) != NULL) {
                int x, y, z;
                if (sscanf(input, "%d %d %d", &x, &y, &z) == 3) {
                    if (math_logic_challenge(x, y, z)) {
                        printf("FH2_FLAG_SEVEN_4408\n");
                    } else {
                        printf("Mathematical constraints not satisfied.\n");
                    }
                } else {
                    printf("Invalid input format.\n");
                }
            }
            break;
            
        case 8:
            printf("Performing heap analysis...\n");
            heap_challenge();
            break;
            
        case 9:
            printf("Enter protocol identifier: ");
            if (fgets(input, sizeof(input), stdin) != NULL) {
                input[strcspn(input, "\n")] = 0;
                if (pcap_decoder_challenge(input)) {
                    printf("FH2_FLAG_NINE_5177\n");
                } else {
                    printf("Protocol validation failed.\n");
                }
            }
            break;
            
        case 10:
            printf("Enter hash chain input: ");
            if (fgets(input, sizeof(input), stdin) != NULL) {
                input[strcspn(input, "\n")] = 0;
                if (multi_stage_hash(input)) {
                    printf("FH2_FLAG_TEN_8288\n");
                } else {
                    printf("Hash chain validation failed.\n");
                }
            }
            break;
            
        default:
            printf("Unknown module selected.\n");
            break;
    }
}