#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum match_type {
  normal = 1,
  zero_or_more,
};

struct match_unit {
  match_type type;
  char val;
};

// p has no ".*", it is either pure string, or string with '*'
static bool is_match_internal(string s, vector<match_unit> p, int *length)
{
  if (p.empty()) {
    *length = 0;
    return true;
  }

  /* calculate nextval[] array */
  vector<int> nextval(p.size());
  nextval[0] = 0; // initialization
  vector<int>::iterator visit_it = nextval.begin()+1, value_it = nextval.begin();
  while (visit_it != nextval.end()) {
    if (value_it == nextval.begin() || p[visit_it-nextval.begin()].val == p[value_it-nextval.begin()].val) {
      *visit_it = nextval[value_it-nextval.begin()];
      visit_it++, value_it++;
    }
    else {
      value_it = nextval.begin()+*value_it;
    }
  }

  vector<match_unit>::iterator patern_it = p.begin();
  int len = 0;
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == patern_it->val || patern_it->val == '.') {
      if (patern_it+1 == p.end()) {
        *length = len;
        return true;
      }
      if (patern_it->type == zero_or_more) {
        len++;
        continue;
      }
      patern_it++;
      len++;
    }
    else if (patern_it->type == zero_or_more) // *patern_it doesn't match
      patern_it++, i--;
    else {
      len = 0;
      patern_it -= nextval[patern_it-p.begin()];
    }
  }
  *length = 0;
  return false;
}

class Solution {
public:
  static bool isMatch(string s, string p) {
    vector<vector<match_unit>> patern_group(1);

    // process p
    bool multi_flag = false;
    int groups = 0;
    for (int i = p.size()-1; i >= 0; i--) {
      if (multi_flag) { // if multi_flag is set, don't care what p[i] is 
        if (p[i] == '.') {
          patern_group.resize(++groups);
          multi_flag = false;
          continue;
        }
        patern_group[groups].insert(patern_group[groups].begin(), {zero_or_more, p[i]});
        multi_flag = false;
      }
      else if (p[i] != '*') {
        patern_group[groups].insert(patern_group[groups].begin(), {normal, p[i]});
      }
      else 
        multi_flag = true;
    }
    
    /* now vector patern has no multiple signs, and '*' is real '*' */
    int end_index = 0, substr_length = 0;
    for (int i = 0; i < patern_group.size(); i++) {
      // find patern_slice in the according substring
      if (is_match_internal(s.substr(end_index), patern_group[i], &substr_length) != true) {
        return false;
      }
    }
    
    return true;
  }
};
int main(int argc, char **argv)
{
  if (argc != 3) {
    return 0;
  }
  string s(argv[1]), p(argv[2]);
  cout << "s: " << s << endl << "p: " << p << endl;
  if (Solution::isMatch(s, p)) {
    cout << "matched" << endl;
  }
  else 
    cout << "does not match" << endl;
  return 0;
}
