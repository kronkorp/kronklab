#include "private.h"
#include <kronklab/kronklab.h>
#include <signal.h>
#include <stdio.h>

void run_test(
    struct __kl_funcStruct *current,
    unsigned long *results
)
{
    timestamp now = kl_monotonic();
    int sig = 0;
    int code = kl_run_child(current, &sig);

    if (code == -1) {
        klError("Failed to run test %s:%s\n", current->suite, current->name);
        ++results[2];
        return;
    } else {
        if (sig) {
            int sign = sig;
            if (sign == SIGSEGV) {
                klError("Segmentation fault while running %s:%s\n", current->suite, current->name);
            } 
            else if (sign == SIGABRT) {
                klError("Test %s:%s aborted (Standard assert failed or abort() called)\n", current->suite, current->name);
            } 
            else if (sign == SIGFPE) {
                klError("Floating point exception (Division by zero) in %s:%s\n", current->suite, current->name);
            } 
            else {
                klError("Test %s:%s killed by signal %d\n", current->suite, current->name, sign);
            }
            ++results[3];
        } else {
            if (code == 0) {
                klGood("Test %s:%s runned successfully. Took %zu ms.\n", current->suite, current->name, kl_monotonic() - now);
                ++results[1];
            } else {
                ++results[2];
            }
        }
    }
}
