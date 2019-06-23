#include "read.h"

int read (const char *filename, global_list *head, tree_name *root_name, tree_phone *root_phone, group_group *hash_group)
{
  FILE *file_input;
  char name[MAX_LEN_NAME];
  int phone, group, idx = 0;
  record_node *curr = 0, *next = 0;

  if (!(file_input = fopen (filename, "r")))
    {
      return -1;
    }

  if (fscanf (file_input, "%s%d%d", name, &phone, &group) == 3)
    {
      if (phone < MIN_PHONE && phone > MAX_PHONE &&
          group < MIN_GROUP && group > MAX_GROUP)
        {
          return -2;
        }
      idx++;
      curr = new record_node (name, phone, group);
      int was_add = 0;
      root_name->add_new_node (root_name->get_root (), curr, &was_add);
      root_phone->add_new_node (root_phone->get_root (), curr, &was_add);
      (hash_group + curr->get_hash ())->add_to_tail (curr);
      head->set_head (curr);
      curr->set_global_prev (NULL);
    }
  while (fscanf (file_input, "%s%d%d", name, &phone, &group) == 3)
    {
      if (phone < MIN_PHONE && phone > MAX_PHONE &&
          group < MIN_GROUP && group > MAX_GROUP)
        {
          return -2;
        }
      next = new record_node (name, phone, group);
      int was_add = 0;
      root_name->add_new_node (root_name->get_root (), next, &was_add);
      if (!was_add)
        {
          delete next;
          continue;
        }
      root_phone->add_new_node (root_phone->get_root (), next, &was_add);
      (hash_group + curr->get_hash ())->add_to_tail (curr);
      curr->set_global_next (next);
      next->set_global_prev (curr);
      curr = next;
      idx++;
    }
  curr->set_global_next (NULL);
  head->set_tail (curr);
  if (!feof (file_input))
    {
      fclose (file_input);
      return -2;
    }
  fclose (file_input);
  return idx;
}


void print_list (group_name *head, FILE **outputfile, char *space)
{
  record_node *curr;
  for (curr = head->get_head(); curr; curr = curr->get_name_next())
    {
      fprintf (*outputfile, "%s %s %d %d\n", space, curr->get_name(), curr->get_phone(), curr->get_group());
    }
}

void print_list (group_phone *head, FILE **outputfile, char *space)
{
  record_node *curr;
  for (curr = head->get_head(); curr; curr = curr->get_phone_next())
    {
      fprintf (*outputfile, "%s %s %d %d\n", space, curr->get_name(), curr->get_phone(), curr->get_group());
    }
}

void print_list (global_list *head, int n, FILE **outputfile)
{
  int idx = 0,
      left_board = (n > MAX_PRINTABLE_NUMBER ? MAX_PRINTABLE_NUMBER : n),
      right_board = ((n - left_board) > left_board ? n - left_board : left_board);

  fprintf (*outputfile, "LIST:\n");
  record_node *curr;
  for (curr = head->get_head(); curr; curr = curr->get_global_next())
    {
      if (idx == left_board)
        {
          break;
        }
      fprintf (*outputfile, " %2d: %s %d %d\n", idx++, curr->get_name(), curr->get_phone(), curr->get_group());
    }

  if (2 * MAX_PRINTABLE_NUMBER < n)
    {
      fprintf (*outputfile, "..............................\n");
      fprintf (*outputfile, "..............................\n");
    }
  for (; idx < right_board && curr; curr = curr->get_global_next(), idx++)
    {
      if (idx == right_board)
        {
          break;
        }
    }

  for (; curr; curr = curr->get_global_next())
    {
      if (idx == n)
        {
          break;
        }
      fprintf (*outputfile, " %2d: %s %d %d\n", idx++, curr->get_name(), curr->get_phone(), curr->get_group());
    }
  fprintf (*outputfile, "\n");
}

void print_list (group_group *head, int n, FILE **outputfile)
{
  int idx = 0,
      left_board = (n > MAX_PRINTABLE_NUMBER_LESS ? MAX_PRINTABLE_NUMBER_LESS : n),
      right_board = ((n - left_board) > left_board ? n - left_board : left_board);

  record_node *curr;
  for (curr = head->get_head (); curr; curr = curr->get_group_next (), idx++)
    {
      if (idx == left_board)
        {
          break;
        }
      fprintf (*outputfile, "    %s %d %d\n", curr->get_name(), curr->get_phone(), curr->get_group());
    }

  if (2 * MAX_PRINTABLE_NUMBER_LESS < n)
    {
      fprintf (*outputfile, "    ...............\n");
      fprintf (*outputfile, "    ...............\n");
    }
  for (; idx < right_board && curr; curr = curr->get_group_next(), idx++)
    {
      if (idx == right_board)
        {
          break;
        }
    }

  for (; curr; curr = curr->get_group_next(), idx++)
    {
      fprintf (*outputfile, "    %s %d %d\n", curr->get_name(), curr->get_phone(), curr->get_group());
    }
  fprintf (*outputfile, "\n");
}


static int draw = 0;

void print_tree (group_phone *root_name, int level, int from, FILE **outputfile)
{
  char space[MAX_STR];
  space[0] = 0;
  if (level > MAX_LEVEL)
    {
      return;
    }
  if (root_name)
    {
      draw = (draw << 1);
      if (from == 1)
        {
          draw = draw | 1;
        }
      print_tree (root_name->get_left(), level + 1, 0, outputfile);
      for (int i = 0; i < level; i++)
        {
          if (i == level - 1)
            {
              fprintf (stdout, "     |-----");
              if ((1 << (level - i - 1) & draw) >> (level - i - 1))
                {
                  strcat (space, "           ");
                }
              else
                {
                  strcat (space, "     |     ");
                }
              continue;
            }
          if ((1 << (level - i - 1) & draw) >> (level - i - 1))
            {
              fprintf (stdout, "     |     ");
              strcat (space, "     |     ");
              continue;
            }
          fprintf (stdout, "           ");
          strcat (space, "           ");
        }
      if (level == MAX_LEVEL)
        {
          fprintf (stdout, " . . . \n");
        }
      else
        {
          if (root_name->get_colour () == BLACK)
            {
              printf ("[%d: BLACK]\n", root_name->get_phone());
            }
          else
            {
              printf ("[%d: RED]\n", root_name->get_phone());
            }
          print_list (root_name, outputfile, space);
        }
      draw = (draw >> 1) << 1;
      if (from == 0)
        {
          draw = draw | 1;
        }
      print_tree (root_name->get_right(), level + 1, 1, outputfile);
      draw = draw >> 1;
    }
}

void print_tree (group_name *root_name, int level, int from, FILE **outputfile)
{
  char space[MAX_STR];
  space[0] = 0;
  if (level > MAX_LEVEL)
    {
      return;
    }
  if (root_name)
    {
      draw = (draw << 1);
      if (from == 1)
        {
          draw = draw | 1;
        }
      print_tree (root_name->get_left(), level + 1, 0, outputfile);
      for (int i = 0; i < level; i++)
        {
          if (i == level - 1)
            {
              fprintf (stdout, "     |-----");
              if ((1 << (level - i - 1) & draw) >> (level - i - 1))
                {
                  strcat (space, "           ");
                }
              else
                {
                  strcat (space, "     |     ");
                }
              continue;
            }
          if ((1 << (level - i - 1) & draw) >> (level - i - 1))
            {
              fprintf (stdout, "     |     ");
              strcat (space, "     |     ");
              continue;
            }
          fprintf (stdout, "           ");
          strcat (space, "           ");
        }
      if (level == MAX_LEVEL)
        {
          fprintf (stdout, " . . . \n");
        }
      else
        {
          if (root_name->get_colour () == BLACK)
            {
              printf ("[%s: BLACK]\n", root_name->get_head ()->get_name ());
            }
          else
            {
              printf ("[%s: RED]\n", root_name->get_head ()->get_name ());
            }
          print_list (root_name, outputfile, space);
        }
      draw = (draw >> 1) << 1;
      if (from == 0)
        {
          draw = draw | 1;
        }
      print_tree (root_name->get_right(), level + 1, 1, outputfile);
      draw = draw >> 1;
    }
}


void print_hash_table (group_group *hash, FILE **outputfile)
{
  for (int key_hash = 1; key_hash < MAX_HASH; key_hash++)
    {
      fprintf (*outputfile, "[%d] - %d elements:\n", key_hash, (hash + key_hash)->get_number ());
      print_list (hash + key_hash, (hash + key_hash)->get_number (), outputfile);
    }
}


void print_record (record_node *root_name, FILE **output_file = &stdout)
{
  fprintf (*output_file, "[%s %d %d]\n", root_name->get_name(), root_name->get_phone (), root_name->get_group ());
}

void return_error (int result, const char *file_name)
{
  switch (result)
    {
    case (-1):
      fprintf (stdout, "Cannot open %s\n", file_name);
      break;
    case (-2):
      fprintf (stdout, "Cannot read %s\n", file_name);
      break;
    case (-3):
      fprintf (stdout, "%s is empty\n", file_name);
      break;
    default:
      fprintf (stdout, "Unknown error %d in %s\n", result, file_name);
    }
  return;
}
