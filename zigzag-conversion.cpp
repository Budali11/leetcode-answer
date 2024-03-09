#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Solution {
public:
  static string convert(string s, int numRows) {
    /* when numRows == 1 */
    if (numRows == 1) {
      return s;
    }

    vector<string> sorted(numRows, "");
    string ret;
    int cycle = numRows*2-2;
    for (int i = 0; i < s.size(); i++) {
      if (i % cycle <= numRows-1) {
        sorted[i%cycle].append(1, s[i]);
      }
      else {
        sorted[2*numRows-(i%cycle)-2].append(1, s[i]);
      }
    }
    for (auto i : sorted) 
      ret.append(i);
    return ret;
  }
};

int main(void)
{
  string s = "A";
  cout << Solution::convert(s, 1) << endl;

  return 0;
}

