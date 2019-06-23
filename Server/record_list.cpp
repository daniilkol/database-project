#include "record_list.h"

void record_list::delete_next () //+
{
   record_node *next_curr;

   if (!curr->get_next())
     {
       return;
     }
   next_curr = curr->get_next()->get_next();
   delete curr->get_next();
   curr->set_next (next_curr);
   if (next_curr)
     next_curr->set_prev (curr);
}

void record_list::add_next (record_node *new_element) //+
{
  record_node *next_curr;
  next_curr = curr->get_next();
  curr->set_next (new_element);
  new_element->set_next (next_curr);
}

void record_list::add_to_tail (record_node *new_element)
{
  if (!tail)
    {
      add_first (new_element);
      new_element->set_prev (NULL);
      set_tail (new_element);
      return;
    }
  tail->set_next (new_element);
  new_element->set_prev (tail);
  tail = new_element;
  tail->set_next (NULL);
}

void record_list::add_first (record_node *new_element) //+
{
  new_element->set_next (head);
  head = new_element;
}

void record_list::delete_first () //+
{
  record_node *curr_head;
  curr_head = head->get_next ();
  delete head;
  head = curr_head;
}

void record_list::delete_last () //+
{
  record_node *curr_tail;
  curr_tail = tail->get_prev();
  delete tail;
  tail = curr_tail;
  curr_tail->set_next (NULL);
}
