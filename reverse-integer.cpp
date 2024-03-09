#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <pthread.h>
#include <vector>
using namespace std;

#define DIFFERENCE ('1'-1)
#define itoa(x) ((char)((x)+DIFFERENCE))

class Solution {
public:
  static int reverse(int x) {
    string s;
    vector<int> bits_in_dec(10);
    vector<int> max_dec{2,1,4,7,4,8,3,6,4,7};
    // vector<int> max_dec{7,4,6,3,8,4,7,4,1,2};
    bool isNega = x > 0? false: true;
    for (int i = 0; i < 10; i++) 
      bits_in_dec[i] = (int)(abs(x) / pow(10, i)) % 10;
    // remove unnecessary rows
    for (int i = 9; i >= 0; i--) {
      if (bits_in_dec[i] != 0) {
        bits_in_dec.resize(i+1);
        break;
      }
    }
    for (auto i : bits_in_dec) 
      s.insert(s.end(), itoa(i));
    
    if (bits_in_dec.size() < 10) {
      int ret = atoi(s.c_str());
      if (isNega)
        ret = -ret;
      return ret;
    }
    else {
      for (int i = 0; i < 10; i++) {
        if (bits_in_dec[i] == max_dec[i])
          continue;
        else if (bits_in_dec[i] < max_dec[i]) {
          return atoi(s.c_str()) * (isNega? -1: 1);
        }
        else {
          return 0;
        }
      }
      if (isNega) {
        if (bits_in_dec[0] > 6)
          return 0;
        return -atoi(s.c_str());
      }
      else {
        if (bits_in_dec[0] > 5)
          return 0;
        return atoi(s.c_str());
      }
    }
  }
};


int main(void)
{
  cout << Solution::reverse(1563847412) << endl;
  return 0;
}
