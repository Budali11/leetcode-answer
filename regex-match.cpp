#include <iostream>
#include <pthread.h>
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
  bool operator==(match_unit a) {if (a.type == type && a.val == val) return true; else return false;}
};

vector<vector<match_unit>> str_to_patern(string p);
static vector<int> get_nextval(vector<match_unit> p);
/**
 * @brief calculate a nextval vector according to the passin p
 * @note the returned vector should be used in this way:
 * static bool Search(string s, string p) 
 * {
 *   vector<vector<match_unit>> patern_group = str_to_patern(p);
 *   vector<vector<int>> nextval(patern_group.size());
 *   for (int i = 0; i < patern_group.size(); i++) {
 *     nextval[i] = get_nextval(patern_group[i]);
 *   }
 *   
 *   string::iterator j = p.begin(), i = s.begin();
 *   while (i != s.end()) {
 *     if (*i == *j) {
 *       i++, j++;
 *       if (j == p.end()) 
 *         return true;
 *     }
 *     else { // j jump back to restart match
 *       j = p.begin() + nextval[0][j-p.begin()];
 *       if (j < p.begin()) {
 *         j++, i++;
 *       }
 *     }
 *   }
 *   return false;
 * }
 */
static vector<int> get_nextval(vector<match_unit> p) 
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
    if (p[i].val == p[prefix_len].val) {
      nextval[i] = nextval[prefix_len];
      prefix_len++;
    } 
    else { // when mismatched, namely p[i].val != p[prefix_len].val, we should figure out what nextval[i] and prefix_len should be.
      nextval[i] = prefix_len; //
      while (prefix_len != 0 && p[i].val != p[prefix_len].val) {
        prefix_len = nextval[prefix_len]>0? nextval[prefix_len]: 0;
      }
      if (p[i].val == p[prefix_len].val) 
        prefix_len++;
    }
    i++;
  }
  return nextval;
}

// p must match from the beginning of s, and can leave some characters at the rear of s
static bool entire_match(string s, vector<match_unit> p, int *end_index)
{
  if (s.empty() || p.empty()) {
    *end_index = 0;
    return true;
  }
  vector<match_unit>::iterator patern_it = p.begin();
  for (int i = 0; i < s.size(); i++) {
    if (patern_it == p.end()) {
      *end_index = i;
      return true;
    }
    if (patern_it->val != s[i] && patern_it->val != '.' && patern_it->type != zero_or_more) { // match failed
      return false;
    }
    else if (patern_it->val == s[i] || patern_it->val == '.') { // matched one character
      if (patern_it->type == zero_or_more && i+1 < p.size()) 
        continue;
      patern_it++;
      if (patern_it == p.end()) { // when patern match at the end of s, return here
        *end_index = i;
        return true;
      }
    }
    else // (patern_it->val != s[i] && patern_it->type == zero_or_more)
      patern_it++, i--;
  }
  *end_index = 0;
  return false;
}

// p has no ".*", it is either pure string, or string with '*'
static bool partial_match(string s, vector<match_unit> p, int *end_index, int *substr_length)
{
  if (p.empty()) {
    *end_index = 0;
    *substr_length = 0;
    return true;
  }
  int length_ = 0;
  vector<int> nextval = get_nextval(p);
  vector<match_unit>::iterator patern_it = p.begin();
  for (int i = 0; i < s.size(); i++) {
    if (patern_it == p.end()) {
      *end_index = i;
      return true;
    }
    if (patern_it->val != s[i] && patern_it->val != '.' && patern_it->type != zero_or_more) { // match failed
      // restart matching
      length_-=nextval[patern_it-p.begin()];
      patern_it=p.begin()+nextval[patern_it-p.begin()];
    }
    else if (patern_it->val == s[i] || patern_it->val == '.') { // matched one character
      if (patern_it->type == zero_or_more && i+1 < p.size()) 
        continue;
      patern_it++;
      if (patern_it == p.end()) { // when patern match at the end of s, return here
        *end_index = i;
        return true;
      }
    }
    else // (patern_it->val != s[i] && patern_it->type == zero_or_more)
      patern_it++, i--;
  }
  *end_index = 0;
  return false;
}

vector<vector<match_unit>> str_to_patern(string p) 
{
  vector<vector<match_unit>> patern_group(1);
  bool multi_flag = false;
  for (int i = p.size()-1; i >= 0; i--) { 
    if (multi_flag) { // if multi_flag is set, don't care what p[i] is 
      if (p[i] == '.') {
        patern_group.insert(patern_group.begin(), vector<match_unit>());
        multi_flag = false;
        continue;
      }
      patern_group[0].insert(patern_group[0].begin(), {zero_or_more, p[i]});
      multi_flag = false;
    }
    else if (p[i] != '*') {
      patern_group[0].insert(patern_group[0].begin(), {normal, p[i]});
    }
    else 
      multi_flag = true;
  }

  /* this step is to merge match_unit like "a*a*" to "a*", but "aa*" and "a*a"
   * should not be merged. Instead, a*a should be replaced by "aa*" to fit the match algorithm. */
  for (int i = 0; i < patern_group.size(); i++) {
    if (patern_group[i].empty() == true)
      continue;
    for (vector<match_unit>::iterator it = patern_group[i].begin(); it < patern_group[i].end()-1; it++) {
      if (it->type == zero_or_more && *it == *(it+1))
        patern_group[i].erase(it+1);
    }
  }
  return patern_group;
}

class Solution {
public:
  static bool isMatch(string s, string p) {
    vector<vector<match_unit>> patern_group = str_to_patern(p);

    /* now vector patern has no multiple signs, and '*' is real '*' */
    int end_index = 0, substr_length = 0;
    if (entire_match(s, patern_group[0], &end_index) != true) { // the first must match from the begin of s
      return false;
    }
    for (int i = 1; i < patern_group.size(); i++) {
      // find patern_slice in the according substring
      if (partial_match(s.substr(end_index+1), patern_group[i], &end_index, &substr_length) != true) 
        return false;
      end_index+=substr_length;
    }
    if (end_index != s.size())
      return false; // there is substrings remain after the last match
    return true;
  }
};
int main(int argc, char **argv)
{
  if (argc != 2) {
    return 0;
  }
  string p(argv[1]);
  vector<vector<match_unit>> g = str_to_patern(p);
  vector<vector<int>> nextval(g.size());
  for (int i = 0; i < g.size(); i++) {
    nextval[i] = get_nextval(g[i]);
    for (auto j : nextval[i]) {
      cout << j << ' ';
    }
    cout << endl;
  }
  // if (argc != 3) {
  //   return 0;
  // }
  // string s(argv[1]), p(argv[2]);
  // cout << "s: " << s << endl << "p: " << p << endl;
  // if (Solution::isMatch(s, p)) {
  //   cout << "matched" << endl;
  // }
  // else 
  //   cout << "does not match" << endl;
  return 0;
}
