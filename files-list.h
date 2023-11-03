#pragma once

#include <stdint.h>
#include <time.h>
#include <sys/types.h>

typedef enum { FICHIER, DOSSIER } file_type_t;

typedef struct _files_list_entry {
  char path_and_name[4096];
  struct timespec mtime;
  uint64_t size;
  uint8_t md5sum[16];
  file_type_t entry_type;
  mode_t mode;
  struct _files_list_entry *next;
  struct _files_list_entry *prev;
} files_list_entry_t;

typedef struct {
  struct _files_list_entry *head;
  struct _files_list_entry *tail;
} files_list_t;

void clear_files_list(files_list_t *list);
files_list_entry_t *add_file_entry(files_list_t *list, char *file_path);
int add_entry_to_tail(files_list_t *list, files_list_entry_t *entry);
files_list_entry_t *find_entry_by_name(files_list_t *list, char *file_path, size_t start_of_src, size_t start_of_dest);
void display_files_list(files_list_t *list);
void display_files_list_reversed(files_list_t *list);
