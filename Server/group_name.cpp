#include "group_name.h"
#include "read.h"

int group_name::operator< (const group_name &right_hand_side) const
{
  int cmp = strcmp (key, right_hand_side.key);
  if (cmp < 0)
    {
      return 1;
    }
  return 0;
}

int group_name::operator> (const group_name &right_hand_side) const
{
  int cmp = strcmp (key, right_hand_side.key);
  if (cmp > 0)
    {
      return 1;
    }
  return 0;
}

int group_name::operator== (const group_name &right_hand_side) const
{
  if (strcmp (key, right_hand_side.key))
    {
      return 1;
    }
  return 0;
}

int group_name::operator!= (const group_name &right_hand_side) const
{
  if (strcmp (key, right_hand_side.key) != 0)
    {
      return 1;
    }
  return 0;
}

void group_name::add_to_tail (record_node *new_element)
{
  if (!tail)
    {
      add_first (new_element);
      new_element->set_global_prev (NULL);
      tail = new_element;
      return;
    }
  tail->set_global_next (new_element);
  new_element->set_global_prev (tail);
  tail = new_element;
  tail->set_global_next (NULL);
}

void group_name::add_first (record_node *new_element)
{
  new_element->set_global_next (head);
  if (head)
    {
      head->set_global_prev (new_element);
    }
  head = new_element;
  head->set_name_prev (NULL);
  head->set_name_next (NULL);
}

int group_name::add_to_group (record_node *new_element)
{
  if (!get_tail ())
    {
      add_first (new_element);
      new_element->set_name_prev (NULL);
      set_tail (new_element);
      return 1;
    }
  int new_phone = new_element->get_phone (),
      new_group = new_element->get_group ();
  for (record_node *curr = get_head (); curr; curr = curr->get_name_next ())
    {
      if (curr->get_phone () == new_phone && curr->get_group () == new_group)
        {
          return 0;
        }
    }
  get_tail ()->set_name_next (new_element);
  new_element->set_name_prev (get_tail ());
  new_element->set_name_next (NULL);
  set_tail (new_element);
  return 1;
}

void group_name::delete_first ()
{
  record_node *curr_head;
  curr_head = head->get_name_next ();
  delete head;
  head = curr_head;
}

void group_name::delete_last ()
{
  record_node *curr_tail;
  curr_tail = tail->get_name_prev();
  delete tail;
  tail = curr_tail;
  curr_tail->set_name_next (NULL);
}
