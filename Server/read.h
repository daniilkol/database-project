#ifndef READ_H
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "red_black_tree.h"
#include "global_list.h"
#include "group_group.h"

#define MAX_LEVEL 4
#define MAX_PRINTABLE_NUMBER 10
#define MAX_PRINTABLE_NUMBER_LESS 4
#define MAX_LEN_NAME 15
#define MIN_LEN_NAME 5
#define MAX_PHONE 9999999
#define MIN_PHONE 1000000
#define MAX_GROUP 599
#define MIN_GROUP 100

#define MAX_STR 128
#define MAX_HASH 24


int read (const char *, global_list *head, tree_name *root, tree_phone *root_phone, group_group *hash_group);


void print_record (record_node *root, FILE **output_file);

void print_list (group_name *, FILE **outputfile, char *space);
void print_list (group_phone *head, FILE **outputfile, char *space);
void print_list (global_list *head, int n, FILE **outputfile);
void print_list (group_group *head, int n, FILE **outputfile);

void print_tree (group_phone *root_phone, int level, int from, FILE **outputfile);
void print_tree (group_name *root_name, int level, int from, FILE **outputfile);

void print_hash_table (group_group *hash, FILE **outputfile);



void return_error (int result, const char *file_name);


#endif
