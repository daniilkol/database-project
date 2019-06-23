#include "group_phone.h"
#include "read.h"

int group_phone::operator< (const group_phone &right_hand_side) const
{
  return phone < right_hand_side.phone;
}

int group_phone::operator> (const group_phone &right_hand_side) const
{
  return phone > right_hand_side.phone;
}

int group_phone::operator== (const group_phone &right_hand_side) const
{
  return phone == right_hand_side.phone;
}

int group_phone::operator!= (const group_phone &right_hand_side) const
{
  return phone != right_hand_side.phone;
}

void group_phone::add_to_tail (record_node *new_element)
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

void group_phone::add_first (record_node *new_element)
{
  new_element->set_global_next (head);
  if (head)
    {
      head->set_global_prev (new_element);
    }
  head = new_element;
  head->set_phone_prev (NULL);
  head->set_phone_next (NULL);
}

int group_phone::add_to_group (record_node *new_element)
{
  if (!get_tail ())
    {
      add_first (new_element);
      new_element->set_phone_prev (NULL);
      set_tail (new_element);
      return 1;
    }
  const char *new_name = new_element->get_name ();
  int new_group = new_element->get_group ();
  for (record_node *curr = get_head (); curr; curr = curr->get_phone_next ())
    {
      if (!strcmp (curr->get_name (), new_name) && curr->get_group () == new_group)
        {
          return 0;
        }
    }
  get_tail ()->set_phone_next (new_element);
  new_element->set_phone_prev (get_tail ());
  new_element->set_phone_next (NULL);
  set_tail (new_element);
  return 1;
}

void group_phone::delete_first ()
{
  record_node *curr_head;
  curr_head = head->get_phone_next ();
  delete head;
  head = curr_head;
}

void group_phone::delete_last ()
{
  record_node *curr_tail;
  curr_tail = tail->get_phone_prev();
  delete tail;
  tail = curr_tail;
  curr_tail->set_phone_next (NULL);
}
