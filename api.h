#include <pthread.h>

// #define DEBUG
#define RECENT_CURRENT_SIZE 50
#define MAX_THREADS 1000

extern double target_current;
void start_update_current();
double get_current();

double get_current_each();