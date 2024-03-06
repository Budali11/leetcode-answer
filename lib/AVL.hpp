#if !defined (__AVL_HPP)
#define __AVL_HPP
#include <cstdio>
#include <cstring>
#include <algorithm>

template <typename data_type>
class AVL {
  public:
    struct node {
      node *right_, *left_;
      data_type data_;
      int height_;
    };
    node *AVL_Find(data_type target);
    void AVL_Insert(data_type in);
    bool AVL_Remove(data_type target);
    void AVL_Remove(node **target);
    void AVL_Destroy(void);

    AVL(void) { root_ = nullptr;};
    ~AVL();
  private:
    node *root_;
};

/**
 * @brief update field height_ of root
 * @note use this function in recursive function
 */
template <typename data_type>
static void update_height(typename AVL<data_type>::node *root)
{
  if (root->left_ != nullptr && root->right_ != nullptr) {
    root->height_ = std::max(root->left_->height_, root->right_->height_) + 1;
  }
  else if (root->left_ != nullptr)
    root->height_ = root->left_->height_ + 1;
  else if (root->right_ != nullptr)
    root->height_ = root->right_->height_ + 1;
  else 
    root->height_ = 1;
}

/**
 * @brief check if tree root is a balanced tree
 */
template <typename data_type>
static bool isBalanced(typename AVL<data_type>::node *root)
{
  // condition judgements
  if (root->left_ != nullptr && root->right_ != nullptr) {
    if (abs(root->left_->height_ - root->right_->height_) >= 2) 
      return false;
    else
      return true;
  }
  else if (root->left_ == nullptr && root->right_ == nullptr) // this is a leaf node
    return true;
  else if (root->left_ == nullptr) {
    if (root->right_->height_ >= 2) 
      return false;
    else 
      return true;
  }
  else {
    if (root->left_->height_ >= 2) 
      return false;
    else 
      return true;
  }
}

/**
 * @brief make a unbalanced tree to a balanced tree
 * @param root root is a pointer to pointer to an AVL node, and is a lowest node in the tree
 * @note the root must be a unbalanced node!!!
 */
template <typename data_type>
static void Balance(typename AVL<data_type>::node **root)
{
  if ((*root)->left_ == nullptr) {
    if ((*root)->right_->left_ == nullptr) 
      goto RR;
    goto RL;
  }
  if ((*root)->right_ == nullptr) {
    if ((*root)->left_->right_ == nullptr)
      goto LL;
    goto LR;
  }

  if ((*root)->left_->height_ > (*root)->right_->height_) { // this is L-? condition
    // if program reaches here, then root must have a left child
    if ((*root)->left_->left_->height_ > (*root)->left_->right_->height_) { // this is L-L condition
LL:
      // make (*root)->left to be (*root), (*root)->left_->left_ to be (*root)->left_, (*root) to be (*root_)->right_
      typename AVL<data_type>::node *grandpa = *root, *father = (*root)->left_;
      *root = father;
      grandpa->left_ = father->right_;
      (*root)->right_ = grandpa;

      // update height
      update_height<data_type>(grandpa);
      update_height<data_type>(father);
    }
    else { // this is L-R condition
LR:
      typename AVL<data_type>::node *grandpa = *root, *father = (*root)->left_, *child = (*root)->left_->right_;
      grandpa->left_ = child;
      father->right_ = child->left_;
      child->left_ = father;

      // repeat L-L
      *root = child;
      grandpa->left_ = child->right_;
      child->right_ = grandpa;

      update_height<data_type>(grandpa);
      update_height<data_type>(father);
      // child must be updated after the two
      update_height<data_type>(child);
    }
  }
  else { // same as the if-condition, the root must have a right_ child
    if ((*root)->right_->right_->height_ > (*root)->right_->left_->height_) { // this is R-R condition
RR:
      typename AVL<data_type>::node *grandpa = *root, *father = (*root)->right_;
      *root = father;
      grandpa->right_ = father->left_;
      (*root)->left_ = grandpa;

      update_height<data_type>(grandpa);
      update_height<data_type>(father);
    }
    else { // R-L
RL:
      typename AVL<data_type>::node *grandpa = *root, *father = (*root)->right_, *child = (*root)->right_->left_;
      grandpa->right_ = child;
      father->left_ = child->right_;
      child->right_ = father;

      // repeat R-R
      *root = child;
      grandpa->right_ = child->left_;
      child->left_ = grandpa;

      update_height<data_type>(grandpa);
      update_height<data_type>(father);
      update_height<data_type>(child);
    }
  }
}

/**
 * @brief new a node and insert it to a appropriate position in root
 */
template <typename data_type>
static void insert(typename AVL<data_type>::node **root, data_type in)
{
  if ((*root) != nullptr) { // find where to insert
    if ((*root)->data_ < in) 
      insert(&((*root)->right_), in);
    else 
      insert(&((*root)->left_), in);
  }
  else { // here to insert new node
    typename AVL<data_type>::node *new_node = new typename AVL<data_type>::node;
    memset(new_node, 0, sizeof(typename AVL<data_type>::node));
    new_node->data_ = in;
    new_node->height_ = 1;
    *root = new_node;
    return;
  }
  // here to update height_ for higher node
  update_height<data_type>(*root);

  // detect if *root is a balanced tree
  if (isBalanced<data_type>(*root) == true) 
    return;
  
  // do rotations
  Balance<data_type>(root);
}

template <typename data_type>
typename AVL<data_type>::node *AVL<data_type>::AVL_Find(data_type target)
{
  node *tmp = root_;
  for (;tmp != nullptr;) {
    if (tmp->data_ == target)
      return tmp;
    if (tmp->height_ == 1) // tmp is a leaf node
      return nullptr;
    if (tmp->data_ < target) { // if target exists, then it must be in left sub-tree of tmp
      tmp = tmp->right_;
    }
    else {
      tmp = tmp->left_;
    }
  }
  return nullptr;
}

template <typename data_type>
void AVL<data_type>::AVL_Insert(data_type in) { insert(&root_, in);}

template <typename data_type>
static void remove_internal(typename AVL<data_type>::node **root, data_type *replace) 
{
  // find the node
  if ((*root)->right_ != nullptr) 
    remove_internal<data_type>(&((*root)->right_), replace);
  else {
    // here is the most right node in the left sub-tree 
    *replace = (*root)->data_;
    typename AVL<data_type>::node *del = *root;
    *root = (*root)->left_;
    delete del;
    return ; // target not found
  }

  // update height, because its child may be lower
  update_height<data_type>(*root);

  if (isBalanced<data_type>(*root) == true)
    return ;

  Balance<data_type>(root);
}

template <typename data_type>
static bool remove(typename AVL<data_type>::node **root, data_type target) 
{
  data_type replace;
  // find the node
  if ((*root) == nullptr)
    return false; // target not found
  if ((*root)->data_ < target) {
    if (remove(&((*root)->right_), target) != true) 
      return false;
  }
  else if ((*root)->data_ > target) {
    if (remove(&((*root)->left_), target) != true) 
      return false;
  }
  else {
    // find the most right node in the left sub-tree, 
    // or the most left node in the right sub-tree
    if ((*root)->left_ != nullptr) {
      remove_internal<data_type>(&((*root)->left_), &replace);
      (*root)->data_ = replace;
    }
    else if ((*root)->right_ != nullptr) {
      // here (*root) must be a leaf node
      (*root)->data_ = (*root)->right_->data_;
      (*root)->right_ = (*root)->right_->right_;
      delete (*root)->right_;
    }
    else { // (*root) is a leaf
      delete *root;
      *root = nullptr;
      return true;
    }
  }
  // update height, because its child may be lower
  update_height<data_type>((*root));
  
  if (isBalanced<data_type>(*root) == true)
    return true;

  Balance<data_type>(root);
  return true;
}

template <typename data_type>
bool AVL<data_type>::AVL_Remove(data_type target) { return remove(&this->root_, target);}

template <typename data_type>
void AVL<data_type>::AVL_Remove(typename AVL<data_type>::node **target) 
{
}

template <typename data_type>
static void destroy(typename AVL<data_type>::node *root)
{
  if (root == nullptr)
    return;
  destroy<data_type>(root->left_);
  destroy<data_type>(root->right_);
  delete root;
}

template <typename data_type>
void AVL<data_type>::AVL_Destroy() { destroy<data_type>(root_); root_ = nullptr;}

template <typename data_type>
AVL<data_type>::~AVL() { destroy<data_type>(root_); root_ = nullptr;}

#endif

