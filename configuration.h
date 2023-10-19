#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char source[1024];
    char destination[1024];
    uint8_t processes_count;
    bool is_parallel;
    bool uses_md5;
} configuration_t;

void init_configuration(configuration_t *the_config);
int set_configuration(configuration_t *the_config, int argc, char *argv[]);