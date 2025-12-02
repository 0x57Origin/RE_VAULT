#include "flaghunt2.h"

static int fake_equation_1(int x, int y, int z) {
    return (x * 3 + y * 7 - z * 2) == 42;
}

static int fake_equation_2(int x, int y, int z) {
    return (x - y + z * z) == 100;
}

static int fake_equation_3(int x, int y, int z) {
    return ((x + y) * z) == 200;
}

static int hidden_constraint_1(int x, int y, int z) {
    return (x * 2 + 14) == (y - 9);
}

static int hidden_constraint_2(int x, int y, int z) {
    return (y + z - 37) == (x * 3 + 9);
}

static int hidden_constraint_3(int x, int y, int z) {
    return ((x ^ y) + z) == 63;
}

static void noise_computation_1(int x, int y, int z) {
    volatile int result = 0;
    for (int i = 0; i < x; i++) {
        result += (y * z) % 97;
    }
}

static void noise_computation_2(int x, int y, int z) {
    volatile int result = 1;
    for (int i = 0; i < 10; i++) {
        result *= ((x + y + z) % 13);
    }
}

int math_logic_challenge(int x, int y, int z) {
    noise_computation_1(x, y, z);
    
    if (fake_equation_1(x, y, z)) {
        printf("Fake constraint 1 satisfied.\n");
    }
    
    noise_computation_2(x, y, z);
    
    if (fake_equation_2(x, y, z)) {
        printf("Fake constraint 2 satisfied.\n");
    }
    
    if (fake_equation_3(x, y, z)) {
        printf("Fake constraint 3 satisfied.\n");
    }
    
    if (!hidden_constraint_1(x, y, z)) {
        return 0;
    }
    
    if (!hidden_constraint_2(x, y, z)) {
        return 0;
    }
    
    if (!hidden_constraint_3(x, y, z)) {
        return 0;
    }
    
    return 1;
}