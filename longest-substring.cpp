#include <string>
#include <algorithm>
#include <unordered_set>
#include "lib/AVL.hpp"
using namespace std;

class Solution {
public:
  static int lengthOfLongestSubstring(string s) {
    AVL<char> longest;
    int max_length = 0;
    int start_index = 0;
    for (int i = start_index; i < s.size(); ) {
      if (longest.AVL_Find(s[i]) == nullptr) {
        longest.AVL_Insert(s[i]);
        if (max_length < (i - start_index + 1)) {
          max_length++;
        }
        i++;
      }
      else {
        // repeat
        // find where to restart
        while (1) {
          if (s[start_index] == s[i]) {
            longest.AVL_Remove(s[start_index]);
            start_index++;
            break;
          }
          longest.AVL_Remove(s[start_index]);
          start_index++;
        }
      }
    }
    return max_length;
  }
};


// class Solution {
//   public:
//     static int lengthOfLongestSubstring(string s) {
//       unordered_set<char> substring;
//       int max_length = 0;
//       int start_index = 0;
//       for (int i = start_index; i < s.size(); ) {
//         if (substring.find(s[i]) == substring.end()) { // not found
//           substring.insert(s[i]);
//           if (max_length < (i - start_index + 1)) {
//             max_length++;
//           }
//           i++;
//         }
//         else {
//           // repeat
//           // find where to restart
//           while (1) {
//             if (s[start_index] == s[i]) {
//               substring.erase(s[start_index]);
//               start_index++;
//               break;
//             }
//             substring.erase(s[start_index]);
//             start_index++;
//           }
//         }
//       }
//       return max_length;
//     }
// };

int main(int argc, char **argv)
{
  string s = "gnhfljxmsudswvlxog";
  return Solution::lengthOfLongestSubstring(s);
}

