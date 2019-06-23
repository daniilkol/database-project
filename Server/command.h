#ifndef COMMAND_H
#define COMMAND_H

#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "strings.h"
#include "record_node.h"
#include "network.h"

#define MAX_LENGTH 128

enum command_type
{
  CMD_NONE,
  QUIT,
  SELECT,
  INSERT,
  DELETE,
  STOP
};

enum condition_type
{
  COND_NONE,
  EQ, //=
  NE, //<>
  LT, //<
  GT, //>
  LE, //<=
  GE, //>=
  LIKE //like
};

enum operation_type
{
  OP_NONE,
  AND,
  OR
};

class command: public record_node
{
private:
  command_type type;
  condition_type c_name;
  condition_type c_phone;
  condition_type c_group;
  operation_type operation;
  char select_order[4];

public:
  command (): record_node ()
  {
    type = CMD_NONE;
    c_name = COND_NONE;
    c_phone = COND_NONE;
    c_group = COND_NONE;
    operation = OP_NONE;
  }

  command (char *string)
  {
    init_command (string);
  }

  ~command ()
  {
    type = CMD_NONE;
    c_name = COND_NONE;
    c_phone = COND_NONE;
    c_group = COND_NONE;
    operation = OP_NONE;
  }

  command_type get_command_type ()
  {
    return type;
  }
  condition_type get_condition_for_name ()
  {
    return c_name;
  }
  condition_type get_condition_for_group ()
  {
    return c_group;
  }
  condition_type get_condition_for_phone ()
  {
    return c_phone;
  }
  operation_type get_operation_type ()
  {
    return operation;
  }

  record_node *get_record ()
  {
    return (record_node *)this;
  }
  char *get_select_order ()
  {
    return select_order;
  }

  int init_command (char *string);
  int make_conditions (char **words, int num_of_words);
  void print_command (int sock);
  void print_command (FILE **output);
};

int put_condition (condition_type *condition, char *word);
bool suitable_for_conditions (command *cmd, record_node *student);

#endif // COMMAND_H
