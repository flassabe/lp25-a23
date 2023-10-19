#pragma once

#include <stdint.h>
#include <time.h>

typedef enum { FICHIER, DOSSIER } file_type_t;

typedef struct _files_list_entry {
  char path_and_name[4096];
  struct timespec mtime;
  uint64_t size;
  uint8_t md5sum[16];
  file_type_t entry_type;
  struct _files_list_entry *next;
  struct _files_list_entry *prev;
} files_list_entry_t;

typedef struct {
  struct _files_list_entry *head;
  struct _files_list_entry *tail;
} files_list_t;

void clear_files_list(files_list_t *list);
int add_file_entry(files_list_t *list, char *file_path); // Access file properties
files_list_entry_t *find_entry_by_name(files_list_t *list, char *file_path);
void display_files_list(files_list_t *list);
void display_files_list_reversed(files_list_t *list);
