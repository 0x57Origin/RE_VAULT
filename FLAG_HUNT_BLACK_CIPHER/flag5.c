#include "flaghunt2.h"

static int timing_check(void) {
    clock_t start, end;
    volatile int dummy = 0;
    
    start = clock();
    for (int i = 0; i < 10000; i++) {
        dummy += i;
    }
    end = clock();
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    if (cpu_time > 0.1) {
        return 0;
    }
    
    return 1;
}

static int ptrace_check(void) {
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
        return 0;
    }
    
    return 1;
}

static int fake_check_1(void) {
    FILE* fp = fopen("/proc/self/status", "r");
    if (fp) {
        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, "TracerPid")) {
                fclose(fp);
                return strstr(line, "TracerPid:\t0") != NULL;
            }
        }
        fclose(fp);
    }
    return 1;
}

static int fake_check_2(void) {
    return access("/tmp/debugger_detected", F_OK) != 0;
}

int anti_debug_check(void) {
    if (!timing_check()) {
        printf("Timing anomaly detected.\n");
        return 0;
    }
    
    if (!fake_check_1()) {
        printf("Process tracer detected.\n");
        return 0;
    }
    
    if (!fake_check_2()) {
        printf("Debug marker file found.\n");
        return 0;
    }
    
    if (!ptrace_check()) {
        printf("Debugger attachment detected.\n");
        return 0;
    }
    
    return 1;
}