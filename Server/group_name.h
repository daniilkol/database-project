#ifndef GROUP_NAME_H
#define GROUP_NAME_H

#include <cstdio>
#include <cstdlib>
#include "record_node.h"

class group_name
{
private:
  record_node *head;
  record_node *tail;

  char *key;
  group_name *left;
  group_name *right;
  group_name *parent;
  colour col;
  int flag;
public:
  group_name ()
  {
    key = 0;
    left = 0;
    right = 0;
    parent = 0;
    col = NONE;
    flag = 0;
  }

  group_name (record_node *initial)
  {
    head = initial;
    initial->set_name_prev (NULL);
    initial->set_name_next (NULL);
    tail = initial;
    key = initial->get_name ();
    left = 0;
    right = 0;
    parent = 0;
    flag = 0;
    col = NONE;
  }

  ~group_name ()
  {
    record_node *next;
    if (flag)
      {
        delete [] key;
      }
    for (record_node *cur = get_head (); cur; cur = next)
      {
        next = cur->get_name_next ();
        delete cur;
      }
    head = 0;
    tail = 0;
    key = 0;
    left = 0;
    right = 0;
    parent = 0;
    col = NONE;
  }


  group_name &operator= (group_name &right_hand_side)
  {
    key = right_hand_side.key;
    set_head (right_hand_side.get_head());
    set_tail (right_hand_side.get_tail());
    return *this;
  }

  char *get_key () const
  {
    if (key)
      {
        return key;
      }
    return get_head ()->get_name ();
  }

  void set_key (char *new_name)
  {
    if (flag)
      {
        return;
      }
    if (key == new_name)
      {
        key = new char [BUF_LEN];
        strcpy (key, new_name);
        flag = 1;
        return;
      }
    key = new_name;
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

  group_name *get_left ()
  {
    return left;
  }

  group_name *get_right ()
  {
    return right;
  }

  group_name *get_parent ()
  {
    return parent;
  }

  group_name *get_brother ()
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

  void set_left (group_name *new_left)
  {
    left = new_left;
  }

  void set_right (group_name *new_right)
  {
    right = new_right;
  }

  void set_parent (group_name *new_parent)
  {
    parent = new_parent;
  }

  void set_son (group_name *new_son)
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

  void change_child (group_name *old_child, group_name *new_child)
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

  int operator< (const group_name &right_hand_side) const;
  int operator> (const group_name &right_hand_side) const;
  int operator== (const group_name &right_hand_side) const;
  int operator!= (const group_name &right_hand_side) const;

};

#endif // GROUP_NAME_H
