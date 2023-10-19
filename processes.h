#pragma once

#include "configuration.h"
#include <sys/ipc.h>
#include <sys/types.h>

typedef struct {
    uint8_t processes_count;
    pid_t main_process_pid;
    pid_t source_lister_pid;
    pid_t destination_lister_pid;
    pid_t *source_analyzers_pids;
    pid_t *destination_analyzers_pids;
    key_t shared_key;
    int message_queue_id;
} process_context_t;

int prepare(configuration_t *the_config, process_context_t *p_context);
void process_loop(process_context_t *p_context);
void clean_processes(configuration_t *the_config, process_context_t *p_context);