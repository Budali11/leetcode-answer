#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <stack>
using namespace std;

static double findMedianSortedArrays(vector<int>& nums) 
{
  if (nums.size() == 0)
    return 0;
  int length = nums.size();
  if ((length & 0x1) == 1) { // odd
    return nums[length/2];
  }
  else { // even
    return ((double)(nums[length/2] + nums[length/2-1])) / 2.0;
  }
}

class Solution {
public:
  static double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    if (nums1.size() == 0 || nums2.size() == 0) {
      if (nums1.size() == 0 && nums2.size() == 0)
        return 0;
      else if (nums1.size() == 0) 
        return ::findMedianSortedArrays(nums2);
      else 
        return ::findMedianSortedArrays(nums1);
    }

    // vectors are not empty
    stack<int> S;
    int length = nums1.size() + nums2.size();
    vector<int>::iterator pnums1 = nums1.begin(), pnums2 = nums2.begin();
    for (int i = 0; i < length/2 + 1; i++) {
      if (pnums1 != nums1.end() && pnums2 != nums2.end()) {
        if (*pnums1 < *pnums2) {
          S.push(*pnums1);
          pnums1++;
        }
        else {
          S.push(*pnums2);
          pnums2++;
        }
      }
      else {
        if (pnums1 == nums1.end()) {
          S.push(*pnums2);
          pnums2++;
        }
        else {
          S.push(*pnums1);
          pnums1++;
        }
      }
    }
    // median are stored in S
    if ((length & 0x1) == 1) { // odd
      return S.top();
    }
    else {
      int med1 = S.top(), med2;
      S.pop();
      med2 = S.top();
      return ((double)(med1 + med2)) / 2.0;
    }
  }
};

int main(void)
{
  vector<int> num1{1}, num2{2, 3, 4};
  double ret = Solution::findMedianSortedArrays(num1, num2);
  std::cout << ret << std::endl;
  return 0;
}


