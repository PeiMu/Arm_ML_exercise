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
#include <iostream>

namespace arm_exercise {

template <typename element_type> void destroy_element(element_type &ele);
template <typename element_type, std::size_t N>
void destroy_element(element_type (&ele)[N]);

/*
 * The user interface of Memory Pool.
 * Use template to be suitable for all kinds of data types.
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
    element_type *ret = memory_pool_malloc();
    auto next_of_ret = next_of(ret);
    try {
      /*
       * Maybe the easiest way to check if it has a default construction.
       * */
      if constexpr (std::is_default_constructible<element_type>::value)
        new (ret) element_type();
      next_of(ret) = next_of_ret;
      if (ret == nullptr)
        return ret;
    } catch (...) {
      free(ret);
      throw;
    }
    return ret;
  }

  void destroy(element_type *const chunk) {
    /*
     * The example gives the virtual destructor...
     * */
    destroy_element(*chunk);

    memory_pool_free(chunk);
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

  /*
   * Get the size of size that will be allocated.
   * For alignment purpose, rounding up to the minimum required alignment.
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
        std::lcm(sizeof(element_type), sizeof(void *)) + sizeof(element_type);
    return (std::numeric_limits<std::size_t>::max() - block_size) /
           alloc_size();
  }

  element_type *malloc_need_resize();

  const std::size_t min_alloc_size =
      std::lcm(sizeof(void *), sizeof(element_type));
  const std::size_t min_align = std::lcm(
      std::alignment_of<void *>::value, std::alignment_of<element_type>::value);
};

template <typename element_type>
element_type *MemoryPool<element_type>::malloc_need_resize() {
  std::size_t partition_size = alloc_size();
  auto block_size = static_cast<std::size_t>(
      chunk_num * partition_size +
      std::lcm(sizeof(element_type), sizeof(void *)) + sizeof(element_type));
  char *ptr = (char *)malloc(block_size);
  if (ptr == nullptr) {
    if (chunk_num > 4) {
      chunk_num >>= 1;
      partition_size = alloc_size();
      block_size = static_cast<std::size_t>(
          chunk_num * partition_size +
          std::lcm(sizeof(element_type), sizeof(void *)) +
          sizeof(element_type));
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

  return static_cast<element_type *>(
      SimpleSegregatedStorage::memory_pool_malloc());
}

template <typename element_type> void destroy_element(element_type &ele) {
  if (!std::has_virtual_destructor<element_type>::value)
    ele.~element_type();
}

/*
 * Cannot use pseudo destructor call on an array type.
 * So here we destroy each of element in array separately.
 * */
template <typename element_type, std::size_t N>
void destroy_element(element_type (&ele)[N]) {
  for (auto i = N; i > 0; i--) {
    destroy_element(ele[i]);
  }
}

template <typename element_type> bool MemoryPool<element_type>::purge_memory() {
  MemoryBlock iter = memory_blocks;
  if (!iter.valid())
    return false;

  /*
   * Iterate through all memory blocks
   * */
  void *freed_iter = free_memory;
  const size_t partition_size = alloc_size();
  do {
    auto next = iter.next();
    for (char *i = static_cast<char *>(iter.begin()); i != iter.end();
         i += partition_size) {
      if (i == freed_iter) {
				auto next_node = next_of(freed_iter);
				// all nodes in this link has been released.
	      if (freed_iter > next_node)
		      break;

				freed_iter = next_node;
				continue;
      }
      if (freed_iter == nullptr)
        break;
      destroy_element(reinterpret_cast<element_type &>(i));
    }
    free(iter.begin());
    iter = next;
  } while (iter.valid());

  memory_blocks.invalidate();
  this->free_memory = nullptr;
  return true;
}
} // namespace arm_exercise

#endif // MEMORY_POOL_H
