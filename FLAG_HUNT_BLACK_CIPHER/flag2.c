#include "flaghunt2.h"

func_ptr_t function_table[] = {
    dead_function_1,
    flag_printer_maze_1,
    flag_printer_maze_2,
    dead_function_2,
    redirect_function_1,
    dead_function_3,
    dead_function_4,
    flag_printer_maze_3,
    redirect_function_2,
    flag_printer_maze_4,
    flag_printer_maze_5
};

void dead_function_1(int x) {
    volatile int dummy = x * 97531;
    printf("Path terminated at checkpoint A%d\n", dummy % 1000);
}

void dead_function_2(int x) {
    volatile int dummy = x * 24681;
    printf("Path terminated at checkpoint B%d\n", dummy % 1000);
}

void dead_function_3(int x) {
    volatile int dummy = x * 13579;
    printf("Path terminated at checkpoint C%d\n", dummy % 1000);
}

void dead_function_4(int x) {
    volatile int dummy = x * 86420;
    printf("Path terminated at checkpoint D%d\n", dummy % 1000);
}

static int maze_state = 0;

void flag_printer_maze_1(int x) {
    if (maze_state == 0) {
        maze_state = 1;
        printf("Checkpoint 1 reached.\n");
    } else {
        printf("Invalid state transition.\n");
        maze_state = 0;
    }
}

void flag_printer_maze_2(int x) {
    if (maze_state == 1) {
        maze_state = 2;
        printf("Checkpoint 2 reached.\n");
    } else {
        printf("Invalid state transition.\n");
        maze_state = 0;
    }
}

void flag_printer_maze_3(int x) {
    if (maze_state == 2) {
        maze_state = 3;
        printf("Checkpoint 3 reached.\n");
    } else {
        printf("Invalid state transition.\n");
        maze_state = 0;
    }
}

void flag_printer_maze_4(int x) {
    if (maze_state == 3) {
        maze_state = 4;
        printf("Checkpoint 4 reached.\n");
    } else {
        printf("Invalid state transition.\n");
        maze_state = 0;
    }
}

void flag_printer_maze_5(int x) {
    if (maze_state == 4) {
        printf("FH2_FLAG_TWO_5531\n");
        maze_state = 0;
    } else {
        printf("Invalid state transition.\n");
        maze_state = 0;
    }
}

void redirect_function_1(int x) {
    int choice = (x * 7919) % 11;
    if (choice < 11 && choice >= 0) {
        function_table[choice](x / 1000);
    }
}

void redirect_function_2(int x) {
    int choice = (x * 2543) % 11;
    if (choice < 11 && choice >= 0) {
        function_table[choice](x / 100);
    }
}

void function_maze_entry(int choice) {
    if (choice == 42719) {
        function_table[4](choice);
        function_table[2](choice);
        function_table[7](choice);
        function_table[1](choice);
        function_table[9](choice);
    } else {
        int index = (choice * 1337) % 11;
        function_table[index](choice);
    }
}