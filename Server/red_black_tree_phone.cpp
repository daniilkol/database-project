#include "red_black_tree.h"
#include "read.h"

void delete_tree (group_phone *root)
{
  if (!root)
    {
      return;
    }
  group_phone *left = root->get_left (),
              *right = root->get_right (),
              *parent = root->get_parent ();
  if (left)
    {
      delete_tree (left);
    }
  if (right)
    {
      delete_tree (right);
    }
  if (parent)
    {
      parent->change_child (root, NULL);
    }
  delete root;
  root = 0;
  return;
}

void rotate (group_phone *new_parent, group_phone *new_left, group_phone *new_rignt)
{
  new_parent->set_colour (BLACK);
  new_left->set_colour (RED);
  new_rignt->set_colour (RED);
  new_parent->set_left (new_left);
  new_left->set_parent (new_parent);
  new_parent->set_right (new_rignt);
  new_rignt->set_parent (new_parent);
}

void tree_phone::add_new_node (group_phone *root, record_node *new_node, int *was_add)
{
  if (!this->get_root ())
    {
      group_phone *cur_node = 0;
      cur_node = new group_phone (new_node);
      this->set_root (cur_node);
      cur_node->set_colour (BLACK);
      (*was_add) = 1;
      return;
    }
  int op;
  if (root->get_phone () < new_node->get_phone ())
    {
      op = -1;
    }
  else if (root->get_phone () > new_node->get_phone ())
    {
      op = 1;
    }
  else
    {
      op = 0;
    }

  if (op == 0)
    {
      (*was_add) = root->add_to_group (new_node);
      return;
    }
  group_phone *left = root->get_left (),
              *right = root->get_right ();
  if (left && op > 0)
    {
      add_new_node (left, new_node, was_add);
      return;
    }
  if (right && op < 0)
    {
      add_new_node (right, new_node, was_add);
      return;
    }
  group_phone *cur_node = 0, *cur_parent = root, *cur_uncle = cur_parent->get_brother (), *cur_grandparent = cur_parent->get_parent ();

  cur_node = new group_phone (new_node);

  (*was_add) = 1;
  if (op > 0)
    {
      root->set_left (cur_node);
    }
  else
    {
      root->set_right (cur_node);
    }
  cur_node->set_parent (root);
  cur_node->set_colour (RED);
  while (cur_parent->get_colour () == RED)
    {
      if (!cur_uncle || cur_uncle->get_colour () == BLACK)
        {
          group_phone *grandgrand = cur_grandparent->get_parent ();
          if (!grandgrand)
            {
              if (*cur_parent < *cur_node && *cur_parent > *cur_grandparent)
                {
                  this->set_root (cur_parent);
                  cur_parent->set_parent (NULL);
                  cur_grandparent->set_right (cur_parent->get_left ());
                  group_phone *cur_left = cur_parent->get_left ();
                  if (cur_left)
                    {
                      cur_left->set_parent (cur_grandparent);
                    }
                  rotate (cur_parent, cur_grandparent, cur_node);
                  return;
                }
              if (*cur_parent > *cur_node && *cur_parent < *cur_grandparent)
                {
                  this->set_root (cur_parent);
                  cur_parent->set_parent (NULL);
                  cur_grandparent->set_left (cur_parent->get_right ());
                  group_phone *cur_right = cur_parent->get_right ();
                  if (cur_right)
                    {
                      cur_right->set_parent (cur_grandparent);
                    }
                  rotate (cur_parent, cur_node, cur_grandparent);
                  return;
                }
              if (*cur_parent < *cur_node && *cur_node < *cur_grandparent)
                {
                  this->set_root (cur_node);
                  cur_node->set_parent (NULL);
                  cur_grandparent->set_left (cur_node->get_right ());
                  cur_parent->set_right (cur_node->get_left ());
                  group_phone *cur_left = cur_node->get_left ();
                  if (cur_left)
                    {
                      cur_left->set_parent (cur_parent);
                    }
                  group_phone *cur_right = cur_node->get_right ();
                  if (cur_right)
                    {
                      cur_right->set_parent (cur_grandparent);
                    }
                  rotate (cur_node, cur_parent, cur_grandparent);
                  return;
                }
              if (*cur_parent > *cur_node && *cur_node > *cur_grandparent)
                {
                  this->set_root (cur_node);
                  cur_node->set_parent (NULL);
                  cur_parent->set_left (cur_node->get_right ());
                  cur_grandparent->set_right (cur_node->get_left ());
                  group_phone *cur_left = cur_node->get_left ();
                  if (cur_left)
                    {
                      cur_left->set_parent (cur_grandparent);
                    }
                  group_phone *cur_right = cur_node->get_right ();
                  if (cur_right)
                    {
                      cur_right->set_parent (cur_parent);
                    }
                  rotate (cur_node, cur_grandparent, cur_parent);
                  return;
                }
            }
          else
            {
              if (*cur_parent < *cur_node && *cur_parent > *cur_grandparent)
                {
                  grandgrand->set_son (cur_parent);
                  cur_parent->set_parent (grandgrand);
                  cur_grandparent->set_right (cur_parent->get_left ());
                  group_phone *cur_left = cur_parent->get_left ();
                  if (cur_left)
                    {
                      cur_left->set_parent (cur_grandparent);
                    }
                  rotate (cur_parent, cur_grandparent, cur_node);
                  return;
                }
              if (*cur_parent > *cur_node && *cur_parent < *cur_grandparent)
                {
                  grandgrand->set_son (cur_parent);
                  cur_parent->set_parent (grandgrand);
                  cur_grandparent->set_left (cur_parent->get_right ());
                  group_phone *cur_right = cur_parent->get_right ();
                  if (cur_right)
                    {
                      cur_right->set_parent (cur_grandparent);
                    }
                  rotate (cur_parent, cur_node, cur_grandparent);
                  return;
                }
              if (*cur_parent < *cur_node && *cur_node < *cur_grandparent)
                {
                  grandgrand->set_son (cur_node);
                  cur_node->set_parent (grandgrand);
                  cur_grandparent->set_left (cur_node->get_right ());
                  cur_parent->set_right (cur_node->get_left ());
                  group_phone *cur_left = cur_node->get_left ();
                  if (cur_left)
                    {
                      cur_left->set_parent (cur_parent);
                    }
                  group_phone *cur_right = cur_node->get_right ();
                  if (cur_right)
                    {
                      cur_right->set_parent (cur_grandparent);
                    }
                  rotate (cur_node, cur_parent, cur_grandparent);
                  return;
                }
              if (*cur_parent > *cur_node && *cur_node > *cur_grandparent)
                {
                  grandgrand->set_son (cur_node);
                  cur_node->set_parent (grandgrand);
                  cur_parent->set_left (cur_node->get_right ());
                  cur_grandparent->set_right (cur_node->get_left ());
                  group_phone *cur_left = cur_node->get_left ();
                  if (cur_left)
                    {
                      cur_left->set_parent (cur_grandparent);
                    }
                  group_phone *cur_right = cur_node->get_right ();
                  if (cur_right)
                    {
                      cur_right->set_parent (cur_parent);
                    }
                  rotate (cur_node, cur_grandparent, cur_parent);
                  return;
                }
            }
        }
      else
        {
          cur_parent->set_colour (BLACK);
          cur_uncle->set_colour (BLACK);
          cur_grandparent->set_colour (RED);
          if (cur_grandparent == get_root ())
            {
              cur_grandparent->set_colour (BLACK);
              return;
            }
        }

      cur_node = cur_grandparent;
      cur_parent = cur_node->get_parent ();
      cur_uncle = cur_parent->get_brother ();
      cur_grandparent = cur_parent->get_parent ();
    }
  return;
}

int count_elements_in_tree (group_phone *root)
{
  int left = 0, right = 0;
  if (root->get_left())
    {
      left = count_elements_in_tree (root->get_left());
    }
  if (root->get_right())
    {
      right = count_elements_in_tree (root->get_right());
    }
  return left + right + 1;
}

int count_levels (group_phone *root)
{
  group_phone *left = root->get_left(), *right = root->get_right();
  int left_levels = 0, right_levels = 0;
  if (left)
    {
      left_levels = count_levels (left);
    }
  if (right)
    {
      right_levels = count_levels (right);
    }
  if (left_levels > right_levels)
    {
      return left_levels + 1;
    }
  return right_levels + 1;
}

void tree_phone::delete_element (record_node *node)
{
  group_phone *cur_group = get_root();
  while (cur_group->get_phone () != node->get_phone ())
    {
      if (cur_group->get_phone () < node->get_phone ())
        {
          cur_group = cur_group->get_right ();
        }
      else
        {
          cur_group = cur_group->get_left ();
        }
    }

  if (!node->get_phone_prev ())
    {
      if (!node->get_phone_next())
        {
          cur_group->set_head (NULL);
          cur_group->set_tail (NULL);
          return;
        }
      record_node *new_node = node->get_phone_next ();
      cur_group->set_head (new_node);
      new_node->set_phone_prev (NULL);
      return;
    }
  record_node *prev = node->get_phone_prev (), *next = node->get_phone_next ();
  prev->set_phone_next (next);
  if (next)
    {
      next->set_phone_prev (prev);
      return;
    }
  cur_group->set_tail (prev);
  return;
}

void tree_phone::delete_group (group_phone *node)
{
  group_phone *cur_node = node,
      *parent = node->get_parent (),
      *left = node->get_left (),
      *right = node->get_right (),
      *brother, *bro_left, *bro_right,
      *grandparent;

  if (!right && !left && !parent)
    {
      root = 0;
      delete node;
      node = 0;
      return;
    }
  if (!right)
    {
      if (left)    //left - red
        {
          node->set_phone (left->get_phone ());
          node->set_head (left->get_head ());
          delete left;
          left = 0;
          return;
        }
      cur_node = node;
    }
  else
    {
      cur_node = node->get_right ();
    }
  left = cur_node->get_left ();
  while (left)
    {
      cur_node = left;
      left = cur_node->get_left ();
    }
  node->set_phone (cur_node->get_phone ());
  node->set_head (cur_node->get_head ());
  if (cur_node->get_colour () == RED)
    {
      parent = cur_node->get_parent ();
      parent->change_child (cur_node, NULL);
      delete cur_node;
      cur_node = 0;
      return;
    }
  right = cur_node->get_right ();
  if (right)
    {
      cur_node->set_phone (right->get_phone ());
      cur_node->set_head (right->get_head ());
      cur_node->set_right (NULL);
      delete right;
      right = 0;
      return;
    }
  cur_node->set_colour (NONE);

  cur_node->set_phone (-1);
  while (1)
    {
      parent = cur_node->get_parent ();
      if (!parent)
        {
          cur_node->set_colour (BLACK);
          return;
        }
      brother = cur_node->get_brother ();
      bro_left = brother->get_left ();
      bro_right = brother->get_right ();
      grandparent = parent->get_parent ();
      if (brother->get_colour () == BLACK)
        {
          if (*parent < *brother &&
              (!bro_left || bro_left->get_colour () == BLACK) &&
              (bro_right && bro_right->get_colour () == RED))
            {
              if (!grandparent)
                {
                  root = brother;
                  brother->set_parent (NULL);
                }
              else
                {
                  grandparent->set_son (brother);
                  brother->set_parent (grandparent);
                  if (parent->get_colour () == BLACK)
                    {
                      brother->set_colour (NONE);
                    }
                  else
                    {
                      brother->set_colour (BLACK);
                    }
                }
              brother->set_left (parent);
              parent->set_parent (brother);
              parent->set_colour (RED);
              parent->set_right (bro_left);
              if (bro_left)
                {
                  bro_left->set_parent (parent);
                }
              brother->set_right (bro_right);
              bro_right->set_parent (brother);
              if (brother->get_colour () == BLACK)
                {
                  if (cur_node->get_phone () != -1)
                    {
                      parent->change_child (cur_node, NULL);
                      delete cur_node;
                      cur_node = 0;
                    }
                  else
                    {
                      cur_node->set_colour (BLACK);
                    }
                  return;
                }
              if (cur_node->get_phone () != -1)
                {
                  parent->change_child (cur_node, NULL);
                  delete cur_node;
                }
              else
                {
                  cur_node->set_colour (BLACK);
                }
              cur_node = brother;
              continue;
            }
          if (*parent > *brother &&
              (!bro_right || bro_right->get_colour () == BLACK) &&
              (bro_left && bro_left->get_colour () == RED))
            {
              if (!grandparent)
                {
                  root = brother;
                  brother->set_parent (NULL);
                }
              else
                {
                  grandparent->set_son (brother);
                  brother->set_parent (grandparent);
                  if (parent->get_colour () == BLACK)
                    {
                      brother->set_colour (NONE);
                    }
                  else
                    {
                      brother->set_colour (BLACK);
                    }
                }
              brother->set_right (parent);
              parent->set_parent (brother);
              parent->set_colour (RED);
              parent->set_left (bro_right);
              if (bro_right)
                {
                  bro_right->set_parent (parent);
                }
              brother->set_left (bro_left);
              bro_left->set_parent (brother);
              if (brother->get_colour () == BLACK)
                {
                  if (cur_node->get_phone () != -1)
                    {
                      parent->change_child (cur_node, NULL);
                      delete cur_node;
                      cur_node = 0;
                    }
                  else
                    {
                      cur_node->set_colour (BLACK);
                    }
                  return;
                }
              if (cur_node->get_phone () != -1)
                {
                  parent->change_child (cur_node, NULL);
                  delete cur_node;
                }
              else
                {
                  cur_node->set_colour (BLACK);
                }
              cur_node = brother;
              continue;
            }
          if (*parent < *brother &&
              bro_left && bro_left->get_colour () == RED)
            {
              brother->set_left (bro_left->get_right ());
              if (bro_left->get_right ())
                {
                  bro_left->get_right ()->set_parent (brother);
                }
              parent->set_right (bro_left->get_left ());
              if (bro_left->get_left ())
                {
                  bro_left->get_left ()->set_parent (parent);
                }
              if (!grandparent)
                {
                  root = bro_left;
                  bro_left->set_parent (NULL);
                  bro_left->set_colour (BLACK);
                }
              else
                {
                  grandparent->set_son (bro_left);
                  bro_left->set_parent (grandparent);
                  bro_left->set_colour (parent->get_colour ());
                }
              bro_left->set_left (parent);
              parent->set_parent (bro_left);
              parent->set_colour (BLACK);
              bro_left->set_right (brother);
              brother->set_parent (bro_left);
              if (cur_node->get_phone () != -1)
                {
                  parent->change_child (cur_node, NULL);
                  delete cur_node;
                  cur_node = 0;
                  return;
                }
              cur_node->set_colour (BLACK);
              return;
            }
          if (*parent > *brother &&
              bro_right && bro_right->get_colour () == RED)
            {
              brother->set_right (bro_right->get_left ());
              if (bro_right->get_left ())
                {
                  bro_right->get_left ()->set_parent (brother);
                }
              parent->set_left (bro_right->get_right ());
              if (bro_right->get_right ())
                {
                  bro_right->get_right ()->set_parent (parent);
                }
              if (!grandparent)
                {
                  root = bro_right;
                  bro_right->set_parent (NULL);
                  bro_right->set_colour (BLACK);
                }
              else
                {
                  grandparent->set_son (bro_right);
                  bro_right->set_parent (grandparent);
                  bro_right->set_colour (parent->get_colour ());
                }
              bro_right->set_right (parent);
              parent->set_parent (bro_right);
              parent->set_colour (BLACK);
              bro_right->set_left (brother);
              brother->set_parent (bro_right);
              if (cur_node->get_phone () != -1)
                {
                  parent->change_child (cur_node, NULL);
                  delete cur_node;
                  cur_node = 0;
                  return;
                }
              cur_node->set_colour (BLACK);
              return;
            }
          if ((!bro_left && !bro_right) ||
              (bro_left && bro_left->get_colour () == BLACK && bro_right->get_colour () == BLACK))
            {
              if (parent->get_colour () == BLACK)
                {
                  parent->set_colour (NONE);
                  brother->set_colour (RED);
                  if (cur_node->get_phone () != -1)
                    {
                      parent->change_child (cur_node, NULL);
                      delete cur_node;
                    }
                  else
                    {
                      cur_node->set_colour (BLACK);
                    }
                  cur_node = parent;
                  continue;
                }
              else
                {
                  parent->set_colour (BLACK);
                  brother->set_colour (RED);
                  if (cur_node->get_phone () != -1)
                    {
                      parent->change_child (cur_node, NULL);
                      delete cur_node;
                      cur_node = 0;
                    }
                  else
                    {
                      cur_node->set_colour (BLACK);
                    }
                  return;
                }
            }
        }
      else
        {
          if (*parent < *brother)
            {
              if (!grandparent)
                {
                  root = brother;
                  brother->set_parent (NULL);
                  brother->set_colour (BLACK);
                }
              else
                {
                  grandparent->set_son (brother);
                  brother->set_parent (grandparent);
                  brother->set_colour (parent->get_colour ());
                }
              brother->set_left (parent);
              parent->set_parent (brother);
              parent->set_colour (RED);
              parent->set_right (bro_left);
              if (bro_left)
                {
                  bro_left->set_parent (parent);
                }
              continue;
            }
          if (*parent > *brother)
            {
              if (!grandparent)
                {
                  root = brother;
                  brother->set_parent (NULL);
                  brother->set_colour (BLACK);
                }
              else
                {
                  grandparent->set_son (brother);
                  brother->set_parent (grandparent);
                  brother->set_colour (parent->get_colour ());
                }
              brother->set_right (parent);
              parent->set_parent (brother);
              parent->set_colour (RED);
              parent->set_left (bro_right);
              if (bro_right)
                {
                  bro_right->set_parent (parent);
                }
              continue;
            }
        }
    }
}

