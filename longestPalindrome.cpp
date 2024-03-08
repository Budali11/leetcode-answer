#include <vector>
#include <iostream>
#include <string>
#include <stack>
using namespace std;


class Solution {
public:
  static string longestPalindrome(string s) {
    if (s.empty() == true) 
      return s;
    string::iterator front = s.begin(), back = s.end()-1;
    vector<string> palindromes;
    int retreat_steps = 0;
    while (1) { 
      /* find first same character */
      while (1) {
        if (*front == *back && front != back)
          break;
        while (front != back && *front != *back) 
          back--;
        if (front == back) {
          front++, back = s.end()-1;
          if (front >= back) {
            // find the longest string in palindromes
            string ret;
            if (palindromes.empty()) {
              return s.substr(0, 1);
            }
            for (auto i : palindromes) {
              if (i.size() > ret.size())
                ret = i;
            }
            return ret;
          }
        }
      }

      bool is_found = false;
      while (*front == *back) { // front and back may be a longestPalindrome's front and back
        if (front == back || front == back - 1) { // it is a Palindrome, and is longest
          int length = 2*(retreat_steps+1) - 1 + (back - front);
          palindromes.insert(palindromes.begin(), s.substr(front-s.begin()-retreat_steps, length));
          front-=retreat_steps-1, back = s.end()-1;
          retreat_steps=0;
          is_found = true;

          break;
        }
        front++, back--;
        retreat_steps++;
      }
      if (!is_found) {
        /* restore front when it start the first simultaneous move */
        front-=retreat_steps, back+=retreat_steps-1;
        retreat_steps=0;
        is_found = false;
      }
    }
  }
};


int main(int argc, char **argv)
{
  string s = "xaabacxcabaaxcabaax";
  cout << 
    Solution::longestPalindrome(s) 
    << endl;
  return 0;
}
