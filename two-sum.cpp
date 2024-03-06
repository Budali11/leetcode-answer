#include <pthread.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

int getIndex(vector<int> v, int K) 
{ 
    auto it = find(v.begin(), v.end(), K); 
  
    // If element was found 
    if (it != v.end())
        return (it - v.begin());
    else 
      return -1;
}

class Solution {
  public:
    vector<int> twoSum(vector<int>& nums, int target) {
      if (target == 0) {
        for (int i = 0; i < nums.size(); i++) {
            for (int j = i + 1; j < nums.size(); j++) {
                if (nums[i] + nums[j] == target) {
                    vector<int> ret;
                    ret.push_back(i);
                    ret.push_back(j);
                    return ret;
                }
            }
        }
        return vector<int>(0);
      }
      vector<int>::iterator max, min;
      //find max and min
      max = max_element(nums.begin(), nums.end());
      min = min_element(nums.begin(), nums.end());

      // 5
      // -20 -18 -7 -3 11 14 18 19 23 25 29 30
      // figure out counts of groups to place these nums
      int cnt = 0;
      cnt = 2*ceil(fabs(abs(*max)>abs(*min)? *max: *min)/fabs(target))+1;
      vector<int> groups[cnt];
      for (int i : nums) {
        if (i < 0) {
          groups[(int)floor((float)i/float(target)) + cnt/2].push_back(i);
        }
        else if (i > 0){
          groups[(int)ceil((float)i/float(target))-1 + cnt/2].push_back(i);
        }
        else groups[0 + cnt/2].push_back(i);
        
      }

      // find pair
      int num1, num2;
      for (int i = 0; i < cnt/2; i++) {
        for (int j = 0; j < groups[i].size(); j++) {
          for (int k = 0; k < groups[cnt-1-i].size(); k++) {
            if (groups[i][j]+groups[cnt-1-i][k]==target) {
              num1 = groups[i][j];
              num2 = groups[cnt-1-i][k];
              vector<int> ret{getIndex(nums, num1), getIndex(nums, num2)};
              return ret;
            }
          }
        }
      }

      // find in groups[0+cnt/2]
      for (int j = 0; j < groups[cnt/2].size(); j++) {
        for (int k = j+1; k < groups[cnt/2].size(); k++) {
          if (groups[cnt/2][j]+groups[cnt/2][k]==target) {
            num1 = groups[cnt/2][j];
            num2 = groups[cnt/2][k];
            if (num1 == num2) {
              num1 = getIndex(nums, num1);
              nums[num1] = num2-1;
              num2 = getIndex(nums, num2);
              nums[num1] = nums[num1]+1;
            }
            else {
              num1 = getIndex(nums, num1);
              num2 = getIndex(nums, num2);
            }
            vector<int> ret{num1, num2};
            return ret;
          }
        }
      }

      return vector<int>(0);
    }
};

int main(int argc, char **argv)
{
  Solution s;
  vector<int> nums{0,3,-3,4,-1};

  s.twoSum(nums, -1);

  return 0;
}


