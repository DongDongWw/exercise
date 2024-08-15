#include <sys/types.h>

#include <algorithm>
#include <csignal>
#include <cstddef>
#include <cstring>
#include <functional>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

#include "utils.hpp"

namespace leetcode {
class Solution {
 public:
  // leetcode 28
  int strStr(std::string haystack, std::string needle) {
    if (haystack.empty() || needle.empty()) {
      return 0;
    }
    auto GetNext = [](std::vector<int>& next, std::string& pattern) {
      if (pattern.size() == 0) {
        return;
      }

      int length = pattern.size();
      next[0] = -1;
      int i = 1, j = 0;

      while (i < length) {
        if (j == -1 || pattern[i] == pattern[j]) {
          ++i;
          ++j;
          next[i] = j;
        } else {
          j = next[j];
        }
      }
    };

    // find the first match using next array
    std::vector<int> next(needle.size());
    GetNext(next, needle);
    int n = haystack.size(), m = needle.size();
    int i = 0, j = 0;
    while (i < n && j < m) {
      if (j == -1 || haystack[i] == needle[j]) {
        ++i;
        ++j;
      } else {
        j = next[j];
      }
    }
    if (j == needle.size()) {
      return i - j;
    }

    return -1;
  }

  std::vector<std::vector<std::string>> partition(std::string s) {
    std::unordered_map<std::pair<int, int>, bool, HashPair> valid_sub_str;
    std::vector<std::vector<std::string>> ans;
    std::vector<std::string> sta;
    int length = s.size();
    std::function<void(std::vector<std::string>&, int)> dfs =
        [&](std::vector<std::string>& sta, int start_idx) -> void {
      if (start_idx == length) {
        ans.push_back(sta);
        return;
      }

      for (int idx = start_idx; idx < length; ++idx) {
        auto iter = valid_sub_str.find({start_idx, idx});
        if (iter != valid_sub_str.end() && iter->second == false) {
          continue;
        } else if (iter != valid_sub_str.end() && iter->second == true) {
          sta.emplace_back(s.substr(start_idx, idx - start_idx + 1));
          dfs(sta, idx + 1);
          sta.pop_back();
        } else {
          int sub_str_len = idx - start_idx + 1;
          bool is_valid = true;
          for (int k = 0; k < sub_str_len / 2; ++k) {
            if (s[start_idx + k] != s[start_idx + sub_str_len - 1 - k]) {
              is_valid = false;
              break;
            }
          }
          valid_sub_str[{start_idx, idx}] = is_valid;
          if (is_valid) {
            sta.emplace_back(s.substr(start_idx, sub_str_len));
            dfs(sta, idx + 1);
            sta.pop_back();
          }
        }
      }
    };

    dfs(sta, 0);
    return ans;
  }
  int numTrees(int n) {
    // dynamic programming, dp[i] = dp[i - 1] * dp[0] + dp[i - 2] * dp[1] +
    // ...
    int dp[n + 1];
    std::memset(&dp, 0, sizeof(dp));
    dp[0] = 1;
    for (size_t i = 1; i <= n; ++i) {
      for (size_t j = 0; j < i; ++j) {
        dp[i] += (dp[j] * dp[i - j - 1]);
      }
    }

    return dp[n];
  }

  int maxPointsInsideSquare(std::vector<std::vector<int>>& points,
                            std::string s) {
    size_t num_of_c = s.size();
    std::vector<int> points_sorted = std::vector(num_of_c, 0);
    std::iota(points_sorted.begin(), points_sorted.end(), 0);
    std::ranges::sort(points_sorted, [&points](int left, int right) {
      return std::min(std::abs(points[left][0]), std::abs(points[left][1])) <
             std::min(std::abs(points[right][0]), std::abs(points[right][1]));
    });
    int rec[26], ans = 0;
    std::memset(rec, 0, sizeof(rec));
    for (auto p : points_sorted) {
      int idx = s.at(p) - 'a';
      if (rec[idx] != 0) {
        return ans;
      }
      ++rec[idx];
      ++ans;
    }
    return ans;
  }

  bool searchMatri(std::vector<std::vector<int>>& matrix, int target) {
    for (const auto& row : matrix) {
      auto iter = std::lower_bound(row.begin(), row.end(), target);
      if (*iter == target) {
        return true;
      }
    }

    return false;
  }

  bool isSubTree(TreeNode* root, TreeNode* subRoot) {
    /*  auto ConvertTree2Vec = [](TreeNode* root, std::vector<int>& vec) {*/
    /*    // pre order*/
    /*    if (root == nullptr) {*/
    /*      return;*/
    /*    }*/
    /*    std::stack<TreeNode*> stk;*/
    /*    stk.emplace(root);*/
    /*    while (!stk.empty()) {*/
    /*      TreeNode* cur = stk.top();*/
    /*      stk.pop();*/
    /*      vec.emplace_back(cur->val);*/
    /*      if (cur->right != nullptr) {*/
    /*        stk.emplace(cur->right);*/
    /*      }*/
    /*      if (cur->left != nullptr) {*/
    /*        stk.emplace(cur->left);*/
    /*      }*/
    /*    }*/
    /*  };*/
    /**/
    /*  std::vector<int> root_vec, sub_root_vec;*/
    /*  ConvertTree2Vec(root, root_vec);*/
    /*  ConvertTree2Vec(subRoot, sub_root_vec);*/
    /**/
    /*  for (size_t i = 0; i < root_vec.size(); ++i) {*/
    /*    for(size_t j = 0; j > sub_root_vec.size(); ++j) {*/
    /*      if (root_vec[i + j] != sub_root_vec[j]) {*/
    /*        break;*/
    /*      }*/
    /**/
    /*      if (j == sub_root_vec.size() - 1) {*/
    /*        return true;*/
    /*      }*/
    /*    }*/
    /*  }*/
    /**/
    /*  return false;*/
    /*}*/
    std::function<bool(TreeNode*, TreeNode*)> IsSameTree =
        [&](TreeNode* node_x, TreeNode* node_y) {
          if (node_x == nullptr) {
            return node_y == nullptr;
          }
          if (node_y == nullptr) {
            return node_x == nullptr;
          }
          return (node_x->val == node_y->val) &&
                 IsSameTree(node_x->left, node_y->left) &&
                 IsSameTree(node_x->right, node_y->right);
        };
    std::function<bool(TreeNode*, TreeNode*)> HasSubTree = [&](TreeNode* root,
                                                               TreeNode* sub) {
      if (root == nullptr) {
        return sub == nullptr;
      }
      return IsSameTree(root, sub) || HasSubTree(root->left, sub) ||
             HasSubTree(root->right, sub);
    };

    return HasSubTree(root, subRoot);
  }

  int diameterOfBinaryTree(TreeNode* root) {
    int max_diam = 0;
    std::function<int(TreeNode*)> dfs = [&](TreeNode* node) -> int {
      if (node == nullptr) {
        return 0;
      }
      int left_depth = dfs(node->left);
      int right_depth = dfs(node->right);
      max_diam = std::max({max_diam, left_depth + right_depth});
      return 1 + std::max(left_depth, right_depth);
    };

    dfs(root);
    return max_diam;
  }

  int orangesrotting(std::vector<std::vector<int>>& grid) {
    int rows = grid.size(), cols = grid[0].size();
    std::queue<std::pair<int, int>> qu;

    int org_nums = 0, rotten_nums = 0;
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        if (grid[i][j] != 0) {
          ++org_nums;
          if (grid[i][j] == 2) {
            qu.emplace(i, j);
          }
          ++rotten_nums;
        }
      }
    }

    auto RottingOneStep = [&](int x, int y) {
      int dx[4] = {0, 0, -1, 1}, dy[4] = {-1, 1, 0, 0};
      for (int k = 0; k < 4; ++k) {
        int next_x = x + dx[k], next_y = y + dy[k];
        if (next_x < 0 || next_y < 0 || next_x >= rows || next_y >= cols) {
          continue;
        }

        if (grid[next_x][next_y] == 1) {
          grid[next_x][next_y] = 2;
          qu.emplace(next_x, next_y);
          ++rotten_nums;
        }
      }
    };

    if (rotten_nums == org_nums) {
      return 0;
    }

    int cost = 0;
    while (!qu.empty()) {
      int num = qu.size();
      for (int i = 0; i < num; ++i) {
        auto [x, y] = qu.front();
        qu.pop();
        RottingOneStep(x, y);
      }
      ++cost;
    }

    return rotten_nums == org_nums ? cost : -1;
  }
  std::vector<std::vector<int>> generate(int numRows) {
    // pascal[row][col] = pascal[row - 1][col - 1] + pascal[row - 1][col - 1]
    std::vector<std::vector<int>> pascal;
    pascal.push_back({1});
    for (int i = 2; i <= numRows; ++i) {
      auto& up_row = pascal.back();
      std::vector<int> cur_row(i, 0);
      for (int j = 0; j < i; ++j) {
        if (j == 0) {
          cur_row[0] = up_row[0];
        } else if (j == i - 1) {
          cur_row[j] = up_row[j - 1];
        } else {
          cur_row[j] = up_row[j] + up_row[j - 1];
        }
      }
      pascal.push_back(cur_row);
    }
    return std::move(pascal);
  }
};

};  // namespace leetcode
