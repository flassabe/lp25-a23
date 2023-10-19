#pragma once

#include <stdbool.h>
#include <files-list.h>
#include <configuration.h>

void synchronize(configuration_t *the_config);
void make_files_list(files_list_t *list, char *target_path);
void copy_entry_to_destination(files_list_entry_t *source_entry);
bool mismatch(files_list_entry_t *lhd, files_list_entry_t *rhd, bool has_md5);
