/*
 * @author: Pei Mu
 * @description: Data structure of memory blocks
 * @data: 30th Jan 2023
 * */

#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <numeric>

namespace arm_exercise {
class MemoryBlock {
public:
  MemoryBlock(void *const ptr, const std::size_t &size)
      : ptr(ptr), size(size) {}
  MemoryBlock() : ptr(nullptr), size(0) {}

  /*
   * A convenient API to get the begin node.
   * */
  void *&begin() { return ptr; }

  void *end() { return ptr_next_ptr(); }

  /*
   * Check if the ptr is empty.
   * */
  bool valid() { return (begin() != nullptr); }

  /*
   * Make the ptr invalid
   * */
  void invalidate() { begin() = nullptr; }

  std::size_t total_size() const { return size; }

  std::size_t element_size() const {
    return static_cast<std::size_t>(
        size - sizeof(std::size_t) -
        std::lcm(sizeof(std::size_t), sizeof(void *)));
  }

  std::size_t &next_size() {
    return *(static_cast<std::size_t *>(static_cast<void *>(ptr_next_size())));
  }

  void *&next_ptr() {
    return *(static_cast<void **>(static_cast<void *>(ptr_next_ptr())));
  }

  MemoryBlock next() { return {MemoryBlock(next_ptr(), next_size())}; }

  void next(MemoryBlock arg) {
    next_ptr() = arg.begin();
    next_size() = arg.total_size();
  }

private:
  void *ptr_next_size() {
    return (static_cast<char *>(ptr) + size - sizeof(std::size_t));
  }

  void *ptr_next_ptr() {
    return (static_cast<char *>(ptr_next_size()) -
            std::lcm(sizeof(std::size_t), sizeof(void *)));
  }

  void *ptr;
  std::size_t size;
};
} // namespace arm_exercise

#endif // MEMORY_BLOCK_H
