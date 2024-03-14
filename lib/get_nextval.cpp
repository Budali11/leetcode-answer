#include <vector>
#include <string>
using namespace std;
static vector<int> get_nextval(string p) 
{
  if (p.empty()) 
    return vector<int>();
  else if (p.size() == 1) {
    return vector<int>(1, 0);
  }
  vector<int> nextval(p.size());
  int prefix_len = 0, i = 1;
  nextval[0] = -1;
  while (i < p.size()) {
    if (p[i] == p[prefix_len]) {
      nextval[i] = nextval[prefix_len];
      prefix_len++;
    } 
    else { // when mismatched, namely p[i] != p[prefix_len], we should figure out what nextval[i] and prefix_len should be.
      nextval[i] = prefix_len; // when search in a string and failed at i,
                               // we should jump to p[prefix_len] to re match, see if p[prefix_len] == p[i]
      
      // here attempt to find what the prefix_len should be when add a new element to nextval[]
      while (prefix_len != 0 && p[i] != p[prefix_len]) { 
        // if the previous prefix_len makes "p[i] == p[prefix_len]" true, then we found the longest prefix_len after adding
        // a new element to nextval[], else find the more previous prefix_len
        prefix_len = nextval[prefix_len]>0? nextval[prefix_len]: 0;
        // if nextval[prefix_len] == -1, then prefix_len should be 0, namely no prefix-suffix pair
      }
      if (p[i] == p[prefix_len]) // prefix_len should count the p[i] and p[prefix_len] pair in
        prefix_len++;
    }
    i++;
  }
  return nextval;
}
