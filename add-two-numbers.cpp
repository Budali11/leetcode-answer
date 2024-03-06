/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
      ListNode *ret = nullptr, *tmp = nullptr, *op1 = l1, *op2 = l2;
      int carry_out = 0;

      // initialize ret
      ret = new ListNode(0);
      tmp = ret;

      // calculate
      for (;;) {
        if (op1 != nullptr && op2 != nullptr) {
          tmp->val = (op1->val + op2->val + carry_out) % 10;
          carry_out = (op1->val + op2->val + carry_out) / 10;
          op1 = op1->next;
          op2 = op2->next;
          if (op1 == nullptr && op2 == nullptr) {
            if (carry_out != 0) 
              tmp->next = new ListNode(carry_out);
            return ret;
          }
          tmp->next = new ListNode(0);
          tmp = tmp->next;
        }
        else {
          ListNode *op = (op1 == nullptr? op2: op1);
          for (;;) {
            tmp->val = (op->val + carry_out) % 10;
            carry_out = (op->val + carry_out) / 10;
            op = op->next;
            if (op == nullptr) {
              if (carry_out != 0) 
                tmp->next = new ListNode(carry_out);
              return ret;
            }
            tmp->next = new ListNode(0);
            tmp = tmp->next;
          }
        }
      }
    }
};
