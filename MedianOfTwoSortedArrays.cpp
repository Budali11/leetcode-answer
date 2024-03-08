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
        // here no need to push elements to S, 
        // because it is already a whole sorted array now
        int med_index = length/2 - S.size();
        if ((length & 0x1) == 1) { // odd
          return (pnums1 == nums1.end())? pnums2[med_index]: pnums1[med_index];
        }
        else { // even
          double ret;
          vector<int>::iterator no_empty = (pnums1 == nums1.end())? pnums2: pnums1;
          if (med_index == 0) {
            ret = ((double)(no_empty[med_index] + S.top())) / 2.0;
            return ret;
          }
          else {
            ret = ((double)(no_empty[med_index] + no_empty[med_index-1])) / 2.0;
            return ret;
          }
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
  vector<int> num1{2}, num2{1, 3, 4, 5};
  double ret = Solution::findMedianSortedArrays(num1, num2);
  std::cout << ret << std::endl;
  return 0;
}


