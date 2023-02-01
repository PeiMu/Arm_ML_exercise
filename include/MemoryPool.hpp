/*
 * @author: Pei Mu
 * @description: Memory pool hpp implementation for ARM's interview exercise
 * @data: 30th Jan 2023
 * */

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "MemoryBlock.hpp"
#include "SimpleSegregatedStorage.hpp"
#include <cassert>
#include <limits>

namespace arm_exercise {
/*
 *
 * */
template <typename element_type>
class MemoryPool : protected SimpleSegregatedStorage {
public:
  explicit MemoryPool(const std::size_t &chunks_num_val = 32,
                      const std::size_t &max_chunks_val = 0)
      : memory_blocks(nullptr, 0), requested_size(sizeof(element_type)) {
    set_chunk_num(chunks_num_val);
    set_max_size(max_chunks_val);
  }

  /*
   * Clean and release all allocated memories when release this class.
   * */
  ~MemoryPool() { purge_memory(); }

  element_type *construct() {
    element_type *const ret = malloc_need_resize();
    if (ret == nullptr)
      return ret;
    try {
      new (ret) element_type();
    } catch (...) {
      free(ret);
      throw;
    }
    return ret;
  }

  void destroy(element_type *const chunk) {
    chunk->~element_type();
    free(chunk);
  }

  /*
   * The interface of malloc.
   * If it's the first time to malloc a trunk,
   *  we need to construct the memory pool first.
   * */
  element_type *memory_pool_malloc() {
    if (this->free_memory != nullptr)
      return static_cast<element_type *>(
          SimpleSegregatedStorage::memory_pool_malloc());
    return malloc_need_resize();
  }

  /*
   * The interface of free.
   * */
  void memory_pool_free(element_type *const chunk) {
    SimpleSegregatedStorage::memory_pool_free(chunk);
  }

protected:
  void set_chunk_num(const std::size_t &next_size_val) {
    chunk_num = std::min(next_size_val, max_chunks());
  }

  void set_max_size(const std::size_t &max_size_val) {
    max_chunk_num = std::min(max_size_val, max_chunks());
  }

  /*
   * Release all memories and reset all variables.
   * */
  bool purge_memory();

  /*
   * The key instant to store the memory pool.
   * */
  MemoryBlock memory_blocks;
  const std::size_t requested_size;
  std::size_t chunk_num{};
  std::size_t max_chunk_num{};

private:
  /*
   * Get the size of size that will be allocated.
   * Rounding up to the minimum required alignment.
   * */
  std::size_t alloc_size() const {
    std::size_t s = std::max(requested_size, min_alloc_size);
    std::size_t rem = s % min_align;
    if (rem)
      s += min_align - rem;
    assert(s >= min_alloc_size);
    assert(s % min_align == 0);
    return s;
  }

  std::size_t max_chunks() const {
    std::size_t block_size =
        std::lcm(sizeof(std::size_t), sizeof(void *)) + sizeof(std::size_t);
    return (std::numeric_limits<std::size_t>::max() - block_size) /
           alloc_size();
  }

  element_type *malloc_need_resize();

  const std::size_t min_alloc_size =
      std::lcm(sizeof(void *), sizeof(std::size_t));
  const std::size_t min_align = std::lcm(std::alignment_of<void *>::value,
                                         std::alignment_of<std::size_t>::value);
};

template <typename element_type>
element_type *MemoryPool<element_type>::malloc_need_resize() {
  std::size_t partition_size = alloc_size();
  auto block_size = static_cast<std::size_t>(
      chunk_num * partition_size +
      std::lcm(sizeof(std::size_t), sizeof(void *)) + sizeof(std::size_t));
  char *ptr = (char *)malloc(block_size);
  if (ptr == nullptr) {
    if (chunk_num > 4) {
      chunk_num >>= 1;
      partition_size = alloc_size();
      block_size = static_cast<std::size_t>(
          chunk_num * partition_size +
          std::lcm(sizeof(std::size_t), sizeof(void *)) + sizeof(std::size_t));
      ptr = (char *)malloc(block_size);
    }
    if (ptr == nullptr)
      return nullptr;
  }

  MemoryBlock node(ptr, block_size);

  if (!max_chunk_num)
    set_chunk_num(chunk_num << 1);
  else if (chunk_num * partition_size / requested_size < max_chunk_num)
    set_chunk_num(std::min(chunk_num << 1,
                           max_chunk_num * requested_size / partition_size));

  this->add_block(node.begin(), node.element_size(), partition_size);

  node.next(memory_blocks);
  memory_blocks = node;

  return this->memory_pool_malloc();
}

template <typename element_type> bool MemoryPool<element_type>::purge_memory() {
  MemoryBlock iter = memory_blocks;
  if (!iter.valid())
    return false;

  /*
   * Iterate through all memory blocks
   * */
  do {
    auto next = iter.next();
    free(iter.begin());
    iter = next;
  } while (iter.valid());

  memory_blocks.invalidate();
  this->free_memory = nullptr;
  return true;
}
} // namespace arm_exercise

#endif // MEMORY_POOL_H
