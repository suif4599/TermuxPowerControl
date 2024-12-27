#include <stdio.h>
#include "api.h"

double target_current;

int main() {
    target_current = 0.7;
    start_update_current();
    printf("%lf", get_current_each());
    return 0;
}