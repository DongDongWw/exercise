#include <functional>
namespace leetcode {
struct TreeNode {
  int val;
  TreeNode* left;
  TreeNode* right;

  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int val) : val(val), left(nullptr), right(nullptr) {}
  TreeNode(int val, TreeNode* left, TreeNode* right)
      : val(val), left(left), right(right) {}
};

struct HashPair {
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U>& p) const {
    return std::hash<T>()(p.first) ^ std::hash<U>()(p.second);
  }
};
};  // namespace leetcode
