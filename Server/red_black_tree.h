#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <cstdio>
#include <cstring>
#include "group_name.h"
#include "group_phone.h"

void delete_tree (group_name *root);
void delete_tree (group_phone *root);

class tree_name
{
private:
  group_name *root;
public:
  tree_name (group_name *new_group = 0)
  {
    root = new_group;
  }
  tree_name (record_node *new_node)
  {
    root = new group_name (new_node);
  }
  tree_name (const tree_name &new_record)
  {
    root = new_record.root;
    root->set_colour (BLACK);
  }
  ~tree_name ()
  {
    if (root)
      {
        delete_tree (root);
      }
    root = 0;
  }

  group_name *get_root()
  {
    return root;
  }

  void set_root (group_name *new_root)
  {
    root = new_root;
    root->set_colour (BLACK);
    root->set_parent (NULL);
  }

  void add_new_node (group_name *root, record_node *new_node, int *was_add);
  void delete_group (group_name *node);
  void delete_element (record_node *node);
};

class tree_phone
{
private:
  group_phone *root;
public:
  tree_phone (group_phone *new_group = 0)
  {
    root = new_group;
  }
  tree_phone (record_node *new_node)
  {
    root = new group_phone (new_node);
  }
  tree_phone (const tree_phone &new_record)
  {
    root = new_record.root;
    root->set_colour (BLACK);
  }
  ~tree_phone ()
  {
    if (root)
      {
        delete_tree (root);
      }
    root = 0;
  }

  group_phone *get_root()
  {
    return root;
  }

  void set_root (group_phone *new_root)
  {
    root = new_root;
    root->set_colour (BLACK);
    root->set_parent (NULL);
  }

  void add_new_node (group_phone *root, record_node *new_node, int *was_add);
  void delete_group (group_phone *node);
  void delete_element (record_node *node);
};


#endif
