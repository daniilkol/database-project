#include <string.h>
#include <cstdlib>
#include "recognize.h"
#include "network.h"

int read_command (global_list *list,
                  tree_name *root_name,
                  tree_phone *root_phone,
                  group_group *hash_group,
                  int *length_of_list,

                  int sock,
                  fd_set &active_set)
{
  char string[MAX_LEN];
  char *commands[MAX_LEN];
  int num_of_commands = 0;
  int result = 0;

  if (read_fd (sock, string) < 0)
    {
      fprintf (stdout, "Too long string\n");
      return -1000;
    }

  string[strlen (string) - 1] = 0;
  num_of_commands = parse (string, ";", commands);
  for (int idx_of_command = 0; idx_of_command < num_of_commands; idx_of_command++)
    {
      command *cmd = new command ();

      int key = cmd->init_command (commands[idx_of_command]);

      if (key <= 0)
        {
          switch (key)
            {
            case (0):
              write_fd (sock, "\n======================================\n");
              write_fd (sock, "Programm exited normally\n");
              break;
            case (-1):
              write_fd (sock, "\n======================================");
              write_fd (sock, "Programm exited normally");
              break;
            case (-11):
              write_fd (sock, "Cannot read [phone] in [insert]!");
              write_fd (sock, "\n======================================");
              write_fd (sock, "Programm returned with Error -11");
              break;
            case (-12):
              write_fd (sock, "Cannot read [group] in [insert]!\n");
              write_fd (sock, "\n======================================\n");
              write_fd (sock, "Programm returned with Error -12\n");
              break;
            case (-21):
              write_fd (sock, "Cannot find [where] in command [select]!!\n");
              write_fd (sock, "\n======================================\n");
              write_fd (sock, "Programm returned with Error -21\n");
              break;
            case (-22):
              write_fd (sock, "Cannot read arguments in command [select]!!\n");
              write_fd (sock, "\n======================================\n");
              write_fd (sock, "Programm returned with Error -22\n");
              break;
            case (-23):
              write_fd (sock, "Cannot read condition in command [select]!!\n");
              write_fd (sock, "\n======================================\n");
              write_fd (sock, "Programm returned with Error -23\n");
              break;
            case (-31):
              write_fd (sock, "Cannot read condition in command [delete]!!\n");
              write_fd (sock, "\n======================================\n");
              write_fd (sock, "Programm returned with Error -31\n");
              break;
            case (-32):
              write_fd (sock, "Cannot find [where] in command [delete]!!\n");
              write_fd (sock, "\n======================================\n");
              write_fd (sock, "Programm returned with Error -32\n");
              break;
            case(-100):
              write_fd (sock, "Cannot recognize command!!\n");
              write_fd (sock, "\n======================================\n");
              write_fd (sock, "Programm returned with Error -100\n");
              break;
            default:
              write_fd (sock, "\n======================================\n");
              write_fd (sock, "Programm returned with Unknown Error\n");
            }
          delete cmd;
          close (sock);
          FD_CLR (sock, &active_set);
          return key;
        }
      write_fd (sock, "......................................");
      cmd->print_command (&stdout);
      cmd->print_command (sock);
      result = run_command (cmd,
                            list,
                            root_name,
                            root_phone,
                            hash_group,
                            length_of_list,
                            sock);
      delete cmd;
    }
  write_fd (sock, "Done");
  write_fd (sock, "......................................");
  write_fd (sock, "done");
  return result;
}

int run_command (command *cmd,
                 global_list *list,
                 tree_name *root_name,
                 tree_phone *root_phone,
                 group_group *hash_group,
                 int *length_of_record_list,
                 int sock)
{
  switch (cmd->get_command_type ())
    {
    case (QUIT):
      return 0;
    case (STOP):
      return -1;

    case (INSERT):
      {
        record_node *new_element = new record_node (*cmd->get_record());
        insert (list,
                root_name,
                root_phone,
                hash_group,
                new_element);
        (*length_of_record_list)++;
        return 1;
      }
    case (SELECT):
      {
        //cmd->print_command (sock);
        if (cmd->get_operation_type () == OR ||
            (cmd->get_condition_for_group () != EQ && cmd->get_condition_for_phone () == COND_NONE && cmd->get_condition_for_name () == COND_NONE) ||
            (cmd->get_condition_for_name () == LIKE && cmd->get_condition_for_group () != EQ && cmd->get_condition_for_phone () == COND_NONE))
          {
            select_list (list, cmd, sock);
          }
        else if (
                 (cmd->get_operation_type () != AND && cmd->get_condition_for_group () == EQ) ||
                 (cmd->get_condition_for_name () == LIKE && cmd->get_condition_for_group () == EQ)
                 )
          {
            select_list (hash_group + cmd->get_hash (), cmd, sock);
          }
        else
          {
            if (cmd->get_condition_for_name () != COND_NONE && cmd->get_condition_for_name () != LIKE)
              {
                select_tree (root_name, cmd, sock);
              }
            else
              {
                select_tree (root_phone, cmd, sock);
              }
          }
        return 1;
      }

    case (DELETE):
      {
        if (cmd->get_condition_for_name() == COND_NONE &&
            cmd->get_condition_for_group() == COND_NONE &&
            cmd->get_condition_for_phone() == COND_NONE)
          {
            delete_all (list, root_name, root_phone, hash_group);
            (*length_of_record_list) = 0;
          }
        else
          {
            if (cmd->get_operation_type () != OR && cmd->get_condition_for_group () == EQ)
              {
                delete_hash_conditions (list,
                                        length_of_record_list,
                                        hash_group + cmd->get_hash (),
                                        root_name,
                                        root_phone,
                                        cmd);
              }
            else
              {
                delete_list_conditions (list,
                                        root_name,
                                        root_phone,
                                        hash_group,
                                        length_of_record_list,
                                        cmd);
              }
          }
        return 1;
      }
    default:
      return -100;
    }
}

void insert (global_list *list,
             tree_name *root_name,
             tree_phone *root_phone,
             group_group *hash_group,
             record_node *new_record)
{
  int was_add = 0;
  root_name->add_new_node (root_name->get_root (), new_record, &was_add);
  if (!was_add)
    {
      delete new_record;
      return;
    }
  root_phone->add_new_node (root_phone->get_root (), new_record, &was_add);
  (hash_group + new_record->get_hash ())->add_to_group (new_record);
  list->add_to_tail (new_record);
  return;
}

void select_list (global_list *list, command *cmd, int sock)
{
  record_node *curr;
  if (!list->get_head())
    {
      return;
    }
  for (curr = list->get_head (); curr; curr = curr->get_global_next ())
    {
      if (suitable_for_conditions (cmd, curr))
        {
          fprint_ (curr, cmd->get_select_order(), sock);
        }
    }
}

void select_list (group_name *list, command *cmd, int sock)
{
  record_node *curr;
  if (cmd->get_condition_for_group () == COND_NONE && cmd->get_condition_for_phone () == COND_NONE)
    {
      for (curr = list->get_head (); curr; curr = curr->get_name_next ())
        {
          fprint_ (curr, cmd->get_select_order(), sock);
        }
      return;
    }

  for (curr = list->get_head (); curr; curr = curr->get_name_next ())
    {
      if (suitable_for_conditions (cmd, curr))
        {
          fprint_ (curr, cmd->get_select_order(), sock);
        }
    }
}

void select_list (group_phone *list, command *cmd, int sock)
{
  record_node *curr;
  if (cmd->get_condition_for_group () == COND_NONE && cmd->get_condition_for_name () == COND_NONE)
    {
      for (curr = list->get_head (); curr; curr = curr->get_phone_next ())
        {
          fprint_ (curr, cmd->get_select_order(), sock);
        }
      return;
    }

  for (curr = list->get_head (); curr; curr = curr->get_phone_next ())
    {
      if (suitable_for_conditions (cmd, curr))
        {
          fprint_ (curr, cmd->get_select_order(), sock);
        }
    }
}

void select_list (group_group *list, command *cmd, int sock)
{
  record_node *curr;
  for (curr = list->get_head (); curr; curr = curr->get_group_next ())
    {
      if (suitable_for_conditions (cmd, curr))
        {
          fprint_ (curr, cmd->get_select_order(), sock);
        }
    }
}


void print_tree_as_list (group_name *root_name, command *cmd, int sock)
{
  if (!root_name)
    {
      return;
    }
  group_name *left = root_name->get_left (), *right = root_name->get_right ();
  if (left)
    {
      print_tree_as_list (left, cmd, sock);
    }
  select_list (root_name, cmd, sock);
  if (right)
    {
      print_tree_as_list (right, cmd, sock);
    }
}

void print_tree_as_list (group_phone *root_phone, command *cmd, int sock)
{
  if (!root_phone)
    {
      return;
    }
  group_phone *left = root_phone->get_left (), *right = root_phone->get_right ();
  if (left)
    {
      print_tree_as_list (left, cmd, sock);
    }
  select_list (root_phone, cmd, sock);
  if (right)
    {
      print_tree_as_list (right, cmd, sock);
    }
}


void select_tree (tree_name *root_name, command *cmd, int sock)
{
  group_name *curr = root_name->get_root();
  switch (cmd->get_condition_for_name ())
    {
    case LE:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            int cmp = strcmp (curr->get_key (), cmd->get_name ());
            if (cmp == 0)
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                select_list (curr, cmd, sock);
                return;
              }
            else if (cmp > 0)
              {
                curr = curr->get_left ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                select_list (curr, cmd, sock);
                curr = curr->get_right ();
                continue;
              }
          }
      }
    case LT:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            int cmp = strcmp (curr->get_key (), cmd->get_name ());
            if (cmp == 0)
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                return;
              }
            else if (cmp > 0)
              {
                curr = curr->get_left ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                select_list (curr, cmd, sock);
                curr = curr->get_right ();
                continue;
              }
          }
      }
    case GE:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            int cmp = strcmp (curr->get_key (), cmd->get_name ());
            if (cmp == 0)
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                select_list (curr, cmd, sock);
                return;
              }
            else if (cmp < 0)
              {
                curr = curr->get_right ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                select_list (curr, cmd, sock);
                curr = curr->get_left();
                continue;
              }
          }
      }
    case GT:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            int cmp = strcmp (curr->get_key (), cmd->get_name ());
            if (cmp == 0)
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                return;
              }
            else if (cmp < 0)
              {
                curr = curr->get_right ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                select_list (curr, cmd, sock);
                curr = curr->get_left ();
                continue;
              }
          }
      }
    case EQ:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            int cmp = strcmp (curr->get_key (), cmd->get_name ());
            if (cmp == 0)
              {
                select_list (curr, cmd, sock);
                return;
              }
            else if (cmp < 0)
              {
                curr = curr->get_right ();
                continue;
              }
            else
              {
                curr = curr->get_left ();
                continue;
              }
          }
      }
    case NE:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            int cmp = strcmp (curr->get_key (), cmd->get_name ());
            if (cmp == 0)
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                print_tree_as_list (curr->get_right (), cmd, sock);
                return;
              }
            else if (cmp < 0)
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                curr = curr->get_right ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                curr = curr->get_left ();
                continue;
              }
          }
      }
    default:
      return;
    }
}

void select_tree (tree_phone *root_phone, command *cmd, int sock)
{
  group_phone *curr = root_phone->get_root ();
  switch (cmd->get_condition_for_phone ())
    {
    case LE:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            if (curr->get_phone () == cmd->get_phone ())
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                select_list (curr, cmd, sock);
                return;
              }
            else if (curr->get_phone () > cmd->get_phone ())
              {
                curr = curr->get_left ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                select_list (curr, cmd, sock);
                curr = curr->get_right ();
                continue;
              }
          }
      }
    case LT:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            if (curr->get_phone () == cmd->get_phone ())
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                return;
              }
            else if (curr->get_phone () > cmd->get_phone ())
              {
                curr = curr->get_left ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                select_list (curr, cmd, sock);
                curr = curr->get_right ();
                continue;
              }
          }
      }
    case GE:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            if (curr->get_phone () == cmd->get_phone ())
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                select_list (curr, cmd, sock);
                return;
              }
            else if (curr->get_phone () < cmd->get_phone ())
              {
                curr = curr->get_right ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                select_list (curr, cmd, sock);
                curr = curr->get_left();
                continue;
              }
          }
      }
    case GT:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            if (curr->get_phone () == cmd->get_phone ())
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                return;
              }
            else if (curr->get_phone () < cmd->get_phone ())
              {
                curr = curr->get_right ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                select_list (curr, cmd, sock);
                curr = curr->get_left ();
                continue;
              }
          }
      }
    case EQ:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            if (curr->get_phone () == cmd->get_phone ())
              {
                select_list (curr, cmd, sock);
                return;
              }
            else if (curr->get_phone () < cmd->get_phone ())
              {
                curr = curr->get_right ();
                continue;
              }
            else
              {
                curr = curr->get_left ();
                continue;
              }
          }
      }
    case NE:
      {
        while (1)
          {
            if (!curr)
              {
                return;
              }
            if (curr->get_phone () == cmd->get_phone ())
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                print_tree_as_list (curr->get_right (), cmd, sock);
                return;
              }
            else if (curr->get_phone () < cmd->get_phone ())
              {
                print_tree_as_list (curr->get_left (), cmd, sock);
                curr = curr->get_right ();
                continue;
              }
            else
              {
                print_tree_as_list (curr->get_right (), cmd, sock);
                curr = curr->get_left ();
                continue;
              }
          }
      }
    default:
      return;
    }
}


void fprint_ (record_node *student, char *order, int sock)
{
  int idx = 0;
  char answer[MAX_LENGTH];
  answer[0] = 0;
  while (idx < 3 && order[idx])
    {
      if (order[idx] == 'N')
        {
          strcat (answer, student->get_name ());
          strcat (answer, " ");
          idx++;
          continue;
        }
      else if (order[idx] == 'P')
        {
          char phone[MAX_LENGTH];
          itoa (student->get_phone (), phone);
          strcat (answer, phone);
          strcat (answer, " ");
          idx++;
          continue;
        }
      else if (order[idx] == 'G')
        {
          char group[MAX_LENGTH];
          itoa (student->get_group (), group);
          strcat (answer, group);
          strcat (answer, " ");
        }
      idx++;
    }
  write_fd (sock, answer);
  return;
}


void delete_all (global_list *list,
                 tree_name *root_name,
                 tree_phone *root_phone,
                 group_group *hash_group)
{
  record_node *head = list->get_head (), *prev, *next;
  while (head)
    {
      list->delete_first();
      prev = head->get_group_prev ();
      if (!prev)
        {
          group_group *cur = hash_group + head->get_hash ();
          next = head->get_group_next ();
          cur->set_head (next);
          next->set_group_prev (NULL);
          head->set_group_next (NULL);
        }
      else
        {

          next = head->get_group_next ();
          prev->set_group_next (next);
          next->set_group_prev (prev);
          head->set_group_next (NULL);
          head->set_group_prev (NULL);
        }
      head = list->get_head ();
    }
  list->set_tail (NULL);
  list->set_head (NULL);
  delete_tree (root_phone->get_root ());
  delete_tree (root_name->get_root ());
}

void delete_list_conditions (global_list *list,
                             tree_name *root_name,
                             tree_phone *root_phone,
                             group_group *hash_group,
                             int *length_of_record_list,
                             command *cmd)
{
  record_node *curr = list->get_head();
  if (!curr)
    {
      return;
    }
  while (curr && curr == list->get_head ())
    {
      if (suitable_for_conditions (cmd, curr))
        {
          list->delete_first ();
          (hash_group + curr->get_hash ())->delete_element (curr);
          root_phone->delete_element (curr);
          root_name->delete_element (curr);
          curr = list->get_head ();
          length_of_record_list--;
        }
      else
        {
          curr = curr->get_global_next();
          break;
        }
    }
  while (curr)
    {
      if (suitable_for_conditions (cmd, curr))
        {
          record_node *next = curr->get_global_next();
          list->delete_element (curr);
          length_of_record_list--;
          (hash_group + curr->get_hash ())->delete_element (curr);
          root_phone->delete_element (curr);
          root_name->delete_element (curr);
          curr = next;
        }
      else
        {
          curr = curr->get_global_next();
        }
    }
}

void delete_hash_conditions (global_list *list,
                             int *length_of_list,
                             group_group *hash_group,
                             tree_name *root_name,
                             tree_phone *root_phone,
                             command *cmd)
{
  record_node *curr = hash_group->get_head();
  if (!curr)
    {
      return;
    }
  while (curr && curr == hash_group->get_head ())
    {
      if (suitable_for_conditions (cmd, curr))
        {
          hash_group->delete_first ();
          list->delete_element (curr);
          length_of_list--;
          root_phone->delete_element (curr);
          root_name->delete_element (curr);
          curr = hash_group->get_head ();
        }
      else
        {
          curr = curr->get_group_next();
          break;
        }
    }
  while (curr)
    {
      if (suitable_for_conditions (cmd, curr))
        {
          record_node *next = curr->get_group_next ();
          hash_group->delete_element (curr);
          list->delete_element (curr);
          length_of_list--;
          root_phone->delete_element (curr);
          root_name->delete_element (curr);
          curr = next;
        }
      else
        {
          curr = curr->get_group_next();
        }
    }
}
