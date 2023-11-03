#pragma once

#include <configuration.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <files-list.h>
#include <stdbool.h>

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

typedef struct {
    int my_recipient_id; // Id of analyzers' MQ topic
    int my_receiver_id; // Id of MQ topic to listen to
    int analyzers_count; // Number of analyzers available
    key_t mq_key;
} lister_configuration_t;

typedef struct {
    int my_recipient_id; // Id of my lister
    int my_receiver_id; // Id I must listen to
    key_t mq_key;
    bool use_md5; // Set to true when computing MD5sum for files
} analyzer_configuration_t;

typedef void (*process_loop_t)(void *);

int prepare(configuration_t *the_config, process_context_t *p_context);
int make_process(process_context_t *p_context, process_loop_t func, void *parameters);
void lister_process_loop(void *parameters);
void analyzer_process_loop(void *parameters);
void clean_processes(configuration_t *the_config, process_context_t *p_context);
void request_element_details(int msg_queue, files_list_entry_t *entry, lister_configuration_t *cfg, int *current_analyzers);