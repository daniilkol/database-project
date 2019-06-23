#ifndef RECORD_LIST_H
#define RECORD_LIST_H

#include "record_node.h"

class record_list
{
private:
  record_node *head;
  record_node *curr;
  record_node *tail;
public

  void add_first (record_node *new_element);
  void add_to_tail (record_node *new_element);

  ~record_list ()
  {
    if (!head)
      {
        head = 0;
        curr = 0;
        tail = 0;
        return;
      }
    head = 0;
    curr = 0;
    tail = 0;
  }

  record_node *get_head ()
  {
    return head;
  }

  record_node *get_curr ()
  {
    return curr;
  }

  record_node *get_tail ()
  {
    return tail;
  }

  void set_head (record_node *new_head)
  {
    head = new_head;
  }

  void set_curr (record_node *new_curr)
  {
    curr = new_curr;
  }

  void set_tail (record_node *new_tail)
  {
    tail = new_tail;
  }
  record_list &operator= (const record_list &right_hand_side)
  {
    head = right_hand_side.head;
    tail = right_hand_side.tail;
    return *this;
  }
};

#endif
