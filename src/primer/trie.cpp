#include "primer/trie.h"
#include <memory>
#include <string_view>
#include <utility>
#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  // throw NotImplementedException("Trie::Get is not implemented.");

  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.
  if (!this->root_) { return nullptr; }
  auto cur = this->root_;
  for (char c : key) {
    if (cur->children_.count(c) == 0) { return nullptr; }
    cur = cur->children_.at(c);
  }
  // 不是终止节点
  if (!cur->is_value_node_) { return nullptr; }
  // 强制转换，查看value的类型是否匹配
  auto node_with_value_ptr = dynamic_cast<const TrieNodeWithValue<T>*>(cur.get());
  if (!node_with_value_ptr) { return nullptr; }
  return node_with_value_ptr->value_.get();
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // throw NotImplementedException("Trie::Put is not implemented.");

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.

  // 空树
  if (!this->root_) { return Trie(std::make_shared<TrieNode>()).Put(key, std::move(value)); }
  // clone 根
  std::shared_ptr<TrieNode> root = this->root_->Clone();
  auto cur = root;
  
  if (key.empty()) {
    // 空key,需要在root插入value
    cur = std::make_shared<TrieNodeWithValue<T>>(cur->children_, std::make_shared<T>(std::move(value)));
    return Trie(cur);
  }
  
  std::size_t i = 0;
  for (; i < key.size() - 1; i++) {
    char c = key[i];
    std::shared_ptr<TrieNode> son;
    if (cur->children_.count(c) == 0) {
      // 不存在对应的儿子时，创建儿子，并更新当前节点的子指针
      son = std::make_shared<TrieNode>();
    } else {
      // 存在儿子，则copy它
      son = cur->children_[c]->Clone();
    }
    cur->children_[c] = son;
    cur = son;
  }

  std::shared_ptr<TrieNodeWithValue<T>> val_node;
  if (cur->children_.count(key[i]) == 0) {
    // 不存在儿子，就创建新的
    val_node = std::make_shared<TrieNodeWithValue<T>>(std::make_shared<T>(std::move(value)));
  } else {
    // 存在儿子，需要copy孙子
    val_node = std::make_shared<TrieNodeWithValue<T>>(cur->children_[key[i]]->children_, std::make_shared<T>(std::move(value)));
  }
  cur->children_[key[i]] = val_node;
  return Trie(root);
}

auto Trie::Remove(std::string_view key) const -> Trie {
  throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
