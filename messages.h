#pragma once

#include <files-list.h>
#include <defines.h>

#define COMMAND_CODE_TERMINATE 0x0
#define COMMAND_CODE_TERMINATE_OK 0x10
#define COMMAND_CODE_ANALYZE_FILE 0x01
#define COMMAND_CODE_FILE_ANALYZED 0x11
#define COMMAND_CODE_ANALYZE_DIR 0x02
#define COMMAND_CODE_FILE_ENTRY 0x12
#define COMMAND_CODE_LIST_COMPLETE 0x22

#define MSG_TYPE_TO_MAIN 1
#define MSG_TYPE_TO_SOURCE_LISTER 2
#define MSG_TYPE_TO_DESTINATION_LISTER 3
#define MSG_TYPE_TO_SOURCE_ANALYZERS 4
#define MSG_TYPE_TO_DESTINATION_ANALYZERS 5

typedef struct {
    long mtype;
    char message;
} simple_command_t;

typedef struct {
    long mtype;
    char op_code; // Contains the analyze file opcode
    files_list_entry_t payload;
} analyze_file_command_t;

typedef struct {
    long mtype;
    char op_code; // Contains the analyze file opcode
    files_list_entry_t payload;
    int reply_to; // MQ id of the sender, to build either source or destination list
} files_list_entry_transmit_t;

typedef struct {
    long mtype;
    char op_code; // Contains the analyze dir opcode
    char target[PATH_SIZE];
} analyze_dir_command_t;

typedef union {
    simple_command_t simple_command;
    analyze_file_command_t analyze_file_command;
    analyze_dir_command_t analyze_dir_command;
    files_list_entry_transmit_t list_entry;
} any_message_t;

int send_analyze_dir_command(int msg_queue, int recipient, char *target_dir);
int send_file_entry(int msg_queue, int recipient, files_list_entry_t *file_entry, int cmd_code);
int send_analyze_file_command(int msg_queue, int recipient, files_list_entry_t *file_entry);
int send_analyze_file_response(int msg_queue, int recipient, files_list_entry_t *file_entry);
int send_files_list_element(int msg_queue, int recipient, files_list_entry_t *file_entry);
int send_list_end(int msg_queue, int recipient);
int send_terminate_command(int msg_queue, int recipient);
int send_terminate_confirm(int msg_queue, int recipient);