#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "time_service.h"

// Implementation of get_time
char **get_time_1_svc(void *argp, struct svc_req *rqstp) {
    static char *result;
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    // Allocate static buffer for the response
    static char buffer[256];
    strftime(buffer, sizeof(buffer), "Current time is: %Y-%m-%d %H:%M:%S", tm_info);

    result = buffer;
    return &result;
}

// Implementation of set_time
void *set_time_1_svc(char **time_str, struct svc_req *rqstp) {
    printf("The time is now: %s\n", *time_str);
    return NULL;
}
