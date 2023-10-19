#pragma once

#include <files-list.h>

#define COMMAND_CODE_TERMINATE 0x0
#define COMMAND_CODE_TERMINATE_OK 0x10
#define COMMAND_CODE_ANALYZE_FILE 0x01
#define COMMAND_CODE_FILE_ANALYZED 0x11

typedef struct {
    long mtype;
    char message;
} simple_command_t;

typedef struct {
    long mtype;
    char op_code; // Contains the analyze file opcode
    files_list_entry_t payload;
} analyze_file_command_t;

typedef union {
    simple_command_t simple_command;
    analyze_file_command_t analyze_file_command;
} any_message_t;