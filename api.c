#include "api.h"
#include <stdio.h>

double VarForComsumption = 12.3;

#ifdef DEBUG
double get_current_each() {
    return 0.3;
}
#else
double get_current_each() {
    FILE* p = popen("termux-battery-status | grep current | grep -o '[0-9]*'", "r");
    if (p == NULL) {
        return 0.0;
    }
    int current;
    fscanf(p, "%d", &current);
    pclose(p);
    return (double)current / 1000000;
}
#endif

double RECENT_CURRENT[RECENT_CURRENT_SIZE];
pthread_t THREADS[MAX_THREADS];
int thread_pointer = 0;

void init_current() {
    for (int i = 0; i < RECENT_CURRENT_SIZE; i++) {
        RECENT_CURRENT[i] = 3.0;
    }
}

double get_current() {
    double sum = 0;
    for (int i = 0; i < RECENT_CURRENT_SIZE; i++) {
        sum += RECENT_CURRENT[i];
    }
    return sum / RECENT_CURRENT_SIZE;
}

void *comsumption(void *arg) {
    while (1) { 
        VarForComsumption = (VarForComsumption * 1.1) / 1.05;
        VarForComsumption = VarForComsumption + (VarForComsumption * 0.01) - (VarForComsumption * 0.005);
        VarForComsumption = VarForComsumption * VarForComsumption / (VarForComsumption + 1);
        VarForComsumption = VarForComsumption - (VarForComsumption / 2) + (VarForComsumption / 3);
    }
    return NULL;
}

void *update_current_block(void *arg) {
    int i = 0;
    double current;
    while (1) {
        current = get_current_each();
        RECENT_CURRENT[i] = current;
        i = (i + 1) % RECENT_CURRENT_SIZE;
        if (get_current() < target_current) {
            if (thread_pointer < MAX_THREADS) {
                pthread_t thread;
                pthread_create(&thread, NULL, comsumption, NULL);
                THREADS[thread_pointer] = thread;
                thread_pointer++;
            }
        } else {
            if (thread_pointer > 0) {
                pthread_cancel(THREADS[--thread_pointer]);
            }
        }
    }
    return NULL;
}

void start_update_current() {
    init_current();
    pthread_t thread;
    pthread_create(&thread, NULL, update_current_block, NULL);
}
