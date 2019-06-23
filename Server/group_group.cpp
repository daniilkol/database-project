#include "group_group.h"

void group_group::add_to_tail (record_node *new_element)
{
  if (!tail)
    {
      add_first (new_element);
      new_element->set_group_prev (NULL);
      tail = new_element;
      return;
    }
  tail->set_group_next (new_element);
  new_element->set_group_prev (tail);
  tail = new_element;
  tail->set_group_next (NULL);
  number++;
  return;
}

void group_group::add_first (record_node *new_element)
{
  new_element->set_group_next (head);
  if (head)
    {
      head->set_group_prev (new_element);
    }
  head = new_element;
  head->set_group_prev (NULL);
  number++;
}

int group_group::add_to_group (record_node *new_element)
{
  if (!get_tail ())
    {
      add_first (new_element);
      new_element->set_group_prev (0);
      new_element->set_group_next (0);
      set_tail (new_element);
      return 1;
    }
  get_tail ()->set_group_next (new_element);
  new_element->set_group_prev (get_tail ());
  new_element->set_group_next (0);
  set_tail (new_element);
  number++;
  return 1;
}

void group_group::delete_first ()
{
  record_node *curr_head;
  curr_head = head->get_group_next ();
  head = curr_head;
  number--;
  if (!head)
    {
      tail = 0;
      head = 0;
      return;
    }
  curr_head->set_group_prev (0);
}

void group_group::delete_last ()
{
  record_node *curr_tail;
  curr_tail = tail->get_group_prev();
  number--;
  if (!curr_tail)
    {
      head = 0;
      tail = 0;
      return;
    }
  tail = curr_tail;
  curr_tail->set_group_next (0);
}

void group_group::delete_element (record_node *curr)
{
  record_node *next_group = curr->get_group_next (), *prev_group = curr->get_group_prev ();
  if (!prev_group)
    {
      //printf ("NOT PREV\n");
      delete_first ();
      return;
    }
  if (next_group)
    {
      next_group->set_group_prev (prev_group);
      prev_group->set_group_next (next_group);
      number--;
    }
  else
    {
      delete_last ();
    }
}
