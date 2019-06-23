#ifndef GROUP_PHONE_H
#define GROUP_PHONE_H

#include <cstdio>
#include <cstdlib>
#include "record_node.h"

class group_phone
{
private:
  record_node *head;
  record_node *tail;

  int phone;
  group_phone *left;
  group_phone *right;
  group_phone *parent;
  colour col;

public:
  group_phone ()
  {
    phone = 0;
    left = 0;
    right = 0;
    parent = 0;
    col = NONE;
  }

  group_phone (record_node *initial)
  {
    head = initial;
    initial->set_phone_prev (NULL);
    initial->set_phone_next (NULL);
    tail = initial;

    phone = initial->get_phone ();

    left = 0;
    right = 0;
    parent = 0;
    col = NONE;
  }

  ~group_phone ()
  {
    head = 0;
    tail = 0;

    phone = 0;
    left = 0;
    right = 0;
    parent = 0;
    col = NONE;
  }


  group_phone &operator= (group_phone &right_hand_side)
  {
    phone = right_hand_side.phone;
    set_head (right_hand_side.get_head());
    set_tail (right_hand_side.get_tail());
    return *this;
  }

  int get_phone ()
  {
    return phone;
  }

  void set_phone (int new_phone)
  {
    phone = new_phone;
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

  group_phone *get_left ()
  {
    return left;
  }

  group_phone *get_right ()
  {
    return right;
  }

  group_phone *get_parent ()
  {
    return parent;
  }

  group_phone *get_brother ()
  {
    if (!parent)
      {
        return NULL;
      }
    if (this == (parent->get_left ()))
      {
        return parent->get_right ();
      }
    else
      {
        return parent->get_left ();
      }
  }

  colour get_colour ()
  {
    return col;
  }

  void set_left (group_phone *new_left)
  {
    left = new_left;
  }

  void set_right (group_phone *new_right)
  {
    right = new_right;
  }

  void set_parent (group_phone *new_parent)
  {
    parent = new_parent;
  }

  void set_son (group_phone *new_son)
  {
    if (*new_son < *this)
      {
        this->set_left (new_son);
      }
    else
      {
        this->set_right (new_son);
      }
  }

  void set_colour (colour new_colour)
  {
    col = new_colour;
  }

  void change_child (group_phone *old_child, group_phone *new_child)
  {

    if (left == old_child)
      {
        left = new_child;
      }
    else
      {
        right = new_child;
      }
    if (new_child)
      {
        col = new_child->get_colour ();
        new_child->set_parent (this);
      }
  }

  void add_to_tail (record_node *new_element);
  void add_first (record_node *new_element);
  int add_to_group (record_node *new_element);
  void delete_first ();
  void delete_last ();

  int operator< (const group_phone &right_hand_side) const;
  int operator> (const group_phone &right_hand_side) const;
  int operator== (const group_phone &right_hand_side) const;
  int operator!= (const group_phone &right_hand_side) const;

};

#endif // GROUP_PHONE_H
