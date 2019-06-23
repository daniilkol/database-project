#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <stdio.h>
#include <stdlib.h>
#include "global_list.h"
#include "red_black_tree.h"
#include "read.h"
#include "command.h"

#define MAX_LEN 128

int read_command (global_list *list,
                  tree_name *root_name,
                  tree_phone *root_phone,
                  group_group *hash_group,
                  int *lengh_of_list,

                  int sock,
                  fd_set &active_set);

int run_command (command *cmd,
                 global_list *list,
                 tree_name *root_name,
                 tree_phone *root_phone,
                 group_group *hash_group,
                 int *length_of_record_list,
                 int socket);

void insert (global_list *list,
             tree_name *root_name,
             tree_phone *root_phone,
             group_group *hash_group,
             record_node *new_record);

void select_list (global_list *list, command *cmd, int socket);

void select_list (group_name *list, command *cmd, int socket);
void select_list (group_phone *list, command *cmd, int socket);
void select_list (group_group *list, command *cmd, int socket);

void select_tree (tree_name *root, command *cmd, int socket);
void select_tree (tree_phone *root_phone, command *cmd, int socket);

void fprint_ (record_node *student, char *order, int socket);
void print_tree_as_list (group_name *root, command *cmd, int socket);
void print_tree_as_list (group_phone *root, command *cmd, int socket);

void delete_all (global_list *list,
                 tree_name *root,
                 tree_phone *root_phone,
                 group_group *hash_group);
void delete_list_conditions (global_list *list,
                             tree_name *root_name,
                             tree_phone *root_phone, group_group *hash_group,
                             int *length_of_record_list,
                             command *cmd);

void delete_hash_conditions (global_list *list,
                             int *length_of_list,
                             group_group *hash_group,
                             tree_name *root_name,
                             tree_phone *root_phone,
                             command *cmd);
#endif // RECOGNIZE_H
