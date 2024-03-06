#if !defined (__BST_HPP)
#define __BST_HPP

template <typename data_type>
class BST {
  public:
    BST* Insert(data_type in);
    bool Delete(BST *node);
    bool Delete(data_type target);
    BST* Find(data_type target);

    BST(data_type data);
    BST *left, *right, *parent;
    int height;
  private:
    data_type data_;
};

#endif

