#ifndef RECORD_NODE_H
#define RECORD_NODE_H

#include <cstdio>
#include <string.h>

#define BUF_LEN 15

enum colour
{
  NONE,
  BLACK,
  RED
};

class record_node
{
private:
  char *name;
  int phone;
  int group;

  record_node *name_next;
  record_node *name_prev;

  record_node *phone_next;
  record_node *phone_prev;

  record_node *group_prev;
  record_node *group_next;

  record_node *global_next;
  record_node *global_prev;


public:
  record_node (const char *initial_name = 0,
               int initial_phone = 0,
               int initial_group = 0)
  {
    init (initial_name, initial_phone, initial_group);

    name_next = 0;
    name_prev = 0;

    phone_next = 0;
    phone_prev = 0;

    global_next = 0;
    global_prev = 0;

    group_prev = 0;
    group_next = 0;
  }

  record_node (record_node &initial_record)
  {
    init (initial_record.name, initial_record.phone, initial_record.group);
  }

  ~record_node ()
  {
    if (name)
      {
        delete [] name;
      }
    name = 0;

    phone = 0;
    group = 0;

    name_next = 0;
    name_prev = 0;

    phone_next = 0;
    phone_prev = 0;

    group_prev = 0;
    group_next = 0;

    global_next = 0;
    global_prev = 0;
  }

  record_node &operator= (const record_node &i)
  {
    phone = i.phone;
    group = i.group;
    if (i.name)
      {
        strcpy (name, i.name);
      }
    else
      {
        name = 0;
      }

    name_next = 0;
    name_prev = 0;

    phone_next = 0;
    phone_prev = 0;

    group_prev = 0;
    group_next = 0;

    global_next = i.global_next;
    global_prev = i.global_prev;
    return *this;
  }

  int init (const char *initial_name = 0, int initial_phone = 0, int initial_group = 0)
  {
    phone = initial_phone;
    group = initial_group;
    if (initial_name)
      {
        name = new char [BUF_LEN];
        if (name)
          {
            strcpy (name, initial_name);
          }
        else
          {
            return 1;
          }
      }
    else
      {
        name = 0;
      }
    return 0;
  }


  int get_group () const
  {
    return group;
  }

  int get_phone () const
  {
    return phone;
  }

  char *get_name () const
  {
    return (char *) name;
  }

  void set_name (const char *new_name)
  {
    if (!name)
      {
        name = new char [BUF_LEN];
        strcpy (name, new_name);
      }
    else
      strcpy (name, new_name);
  }

  void set_phone (int new_phone)
  {
    phone = new_phone;
  }

  void set_group (int new_group)
  {
    group = new_group;
  }


  record_node *get_name_next () const
  {
    return name_next;
  }

  record_node *get_name_prev () const
  {
    return name_prev;
  }

  void set_name_next (record_node *new_next)
  {
    name_next = new_next;
  }

  void set_name_prev (record_node *new_prev)
  {
    name_prev = new_prev;
  }


  record_node *get_phone_next () const
  {
    return phone_next;
  }

  record_node *get_phone_prev () const
  {
    return phone_prev;
  }

  void set_phone_next (record_node *new_next)
  {
    phone_next = new_next;
  }

  void set_phone_prev (record_node *new_prev)
  {
    phone_prev = new_prev;
  }


  record_node *get_group_next () const
  {
    return group_next;
  }

  record_node *get_group_prev () const
  {
    return group_prev;
  }

  void set_group_next (record_node *new_next)
  {
    group_next = new_next;
  }

  void set_group_prev (record_node *new_prev)
  {
    group_prev = new_prev;
  }


  record_node *get_global_next () const
  {
    return global_next;
  }

  record_node *get_global_prev () const
  {
    return global_prev;
  }

  void set_global_next (record_node *new_global_next)
  {
    global_next = new_global_next;
  }

  void set_global_prev (record_node *new_global_prev)
  {
    global_prev = new_global_prev;
  }


  int operator< (const record_node &right_hand_side) const
  {
    int cmp = strcmp (name, right_hand_side.name);
    if (cmp < 0 ||
        (cmp == 0 && (phone < right_hand_side.phone
                      || (phone == right_hand_side.phone && group < right_hand_side.group))))
      {
        return 1;
      }
    return 0;
  }

  int operator> (const record_node &right_hand_side) const
  {
    int cmp = strcmp (name, right_hand_side.name);
    if (cmp > 0 ||
        (cmp == 0 && (phone > right_hand_side.phone
                      || (phone == right_hand_side.phone && group > right_hand_side.group))))
      {
        return 1;
      }
    return 0;
  }

  int operator== (const record_node &right_hand_side) const
  {
    if (phone == right_hand_side.phone && group == right_hand_side.group && strcmp (name, right_hand_side.name) == 0)
      {
        return 1;
      }
    return 0;
  }

  int operator!= (const record_node &right_hand_side) const
  {
    if (phone != right_hand_side.phone || group != right_hand_side.group || strcmp (name, right_hand_side.name) != 0)
      {
        return 1;
      }
    return 0;
  }

  int get_hash ()
  {
    return group / 100 + group % 100 / 10 + group % 10;
  }
};

#endif
