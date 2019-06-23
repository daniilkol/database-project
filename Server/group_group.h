#ifndef GROUP_GROUP_H
#define GROUP_GROUP_H

#include "record_node.h"

class group_group
{
private:
  record_node *head;
  record_node *tail;
  int number;

public:
  group_group ()
  {
    head = 0;
    tail = 0;
    number = 0;
  }

  group_group (record_node *new_head)
  {
    head = new_head;
    tail = new_head;
    number = 1;
  }

  ~group_group ()
  {
    head = 0;
    tail = 0;
    number = 0;
  }

  record_node *get_head () const
  {
    return head;
  }

  record_node *get_tail () const
  {
    return tail;
  }

  void set_head (record_node *new_head)
  {
    head = new_head;
  }

  void set_tail (record_node *new_tail)
  {
    tail = new_tail;
  }

  int get_number () const
  {
    return number;
  }

  void set_number (int new_number)
  {
    number = new_number;
  }

  void add_to_tail (record_node *new_element);
  void add_first (record_node *new_element);
  int add_to_group (record_node *new_element);
  void delete_first ();
  void delete_last ();
  void delete_element (record_node *curr);

};

#endif // GROUP_GROUP_H
