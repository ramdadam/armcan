#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

void LOG_NUMBER(const char* message, uint64_t argument) {
    fprintf(stderr, message, argument);
    fprintf(stderr, "\n");
    fflush(stderr);
    fflush(stdout);
}
void LOG_POINTER(const char* message, void* argument) {
    fprintf(stderr, message, argument);
    fprintf(stderr, "\n");
    fflush(stderr);
    fflush(stdout);
}
void LOG(const char* message) {
    fprintf(stderr, message);
    fprintf(stderr, "\n");
    fflush(stderr);
    fflush(stdout);
}
#endif