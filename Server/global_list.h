#ifndef GLOBAL_LIST_H
#define GLOBAL_LIST_H

#include "record_node.h"

class global_list
{
private:
  record_node *head;
  record_node *tail;
public:
  global_list ()
  {
    head = 0;
    tail = 0;
  }
  global_list (record_node *new_node)
  {
    head = new_node;
    tail = new_node;
  }

  void add_first (record_node *new_element);
  void delete_first ();
  void delete_last ();
  void add_to_tail (record_node *new_element);
  void delete_element (record_node *curr);

  ~global_list ()
  {
    //record_node *next, *cur;
    if (!head)
      {
        head = 0;
        tail = 0;
        return;
      }/*
    for (cur = head; cur; cur = next)
      {
        next = cur->get_global_next();

        delete cur;
      }*/
    head = 0;
    tail = 0;
  }

  record_node *get_head ()
  {
    return head;
  }

  record_node *get_tail ()
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

  global_list &operator= (const global_list &right_hand_side)
  {
    head = right_hand_side.head;
    tail = right_hand_side.tail;
    return *this;
  }
};
#endif // GLOBAL_LIST_H
