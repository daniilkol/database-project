#include "global_list.h"

void global_list::add_to_tail (record_node *new_element)
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

void global_list::add_first (record_node *new_element)
{
  new_element->set_global_next (head);
  head->set_global_prev (new_element);
  head = new_element;
  head->set_global_prev (NULL);
}

void global_list::delete_first ()
{
  record_node *curr_head;
  curr_head = head->get_global_next ();
  if (curr_head)
    {
      head = curr_head;
    }
  else
    {
      head = 0;
      tail = 0;
    }
}

void global_list::delete_last ()
{
  record_node *curr_tail;
  curr_tail = tail->get_global_prev();
  delete tail;
  tail = curr_tail;
  curr_tail->set_global_next (NULL);
}

void global_list::delete_element (record_node *curr)
{
  record_node *next = curr->get_global_next (), *prev = curr->get_global_prev ();
  if (!prev)
    {
      delete_first ();
      return;
    }
  if (next)
    {
      next->set_global_prev (prev);
      prev->set_global_next (next);
    }
  else
    {
      prev->set_global_next (NULL);
      tail = prev;
    }
}
