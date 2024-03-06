#include "BST.hpp"

template <typename data_type>
BST<data_type>* BST<data_type>::Insert(data_type in)
{
  BST *tmp = this;
  BST *parent_ = nullptr;
  // find where to insert 
  for (int i = 0;tmp != nullptr; i++) {
    parent_ = tmp;
    if (tmp->data_ < in) 
      tmp = tmp->right;
    else 
      tmp = tmp->left;
  }

  tmp = new BST<data_type>(in);
  tmp->parent = parent_;
  return tmp;
}

template <typename data_type>
bool BST<data_type>::Delete(BST *node)
{
}

template <typename data_type>
bool BST<data_type>::Delete(data_type target)
{
}

template <typename data_type>
BST<data_type>* BST<data_type>::Find(data_type target)
{
  BST *tmp = this;
  for (;;) {
    if (tmp != nullptr) {
      if (tmp->data_ < target) 
        tmp = tmp->right;
      else if (tmp->data_ > target) 
        tmp = tmp->left;
      else 
        return tmp;
    }
    else
      return nullptr;
  }
}

template <typename data_type>
BST<data_type>::BST(data_type data) : data_(data), height(0){}

