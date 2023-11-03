#pragma once

#include <files-list.h>
#include <stdbool.h>
#include <configuration.h>

int get_file_stats(files_list_entry_t *entry);
int compute_file_md5(files_list_entry_t *entry);
bool directory_exists(char *path_to_dir);
bool is_directory_writable(char *path_to_dir);