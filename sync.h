#pragma once

#include <stdbool.h>
#include <files-list.h>
#include <configuration.h>
#include <processes.h>
#include <dirent.h>

void synchronize(configuration_t *the_config, process_context_t *p_context);
void make_files_list(files_list_t *list, char *target_path);
bool mismatch(files_list_entry_t *lhd, files_list_entry_t *rhd, bool has_md5);
void make_files_lists_parallel(files_list_t *src_list, files_list_t *dst_list, configuration_t *the_config, int msg_queue);
void copy_entry_to_destination(files_list_entry_t *source_entry, configuration_t *the_config);
void make_list(files_list_t *list, char *target);
DIR *open_dir(char *path);
struct dirent *get_next_entry(DIR *dir);