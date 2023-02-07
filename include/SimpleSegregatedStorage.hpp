/*
 * @author: Pei Mu
 * @description: Implementation of simple segregated storage algorithm
 * @data: 30th Jan 2023
 * */

#ifndef SIMPLE_SEGREGATED_STORAGE_H
#define SIMPLE_SEGREGATED_STORAGE_H

#include <cassert>

namespace memory_pool {
/*
 * Using the simple-segregated storage algorithm from boost.
 * Ref:
 * https://www.boost.org/doc/libs/1_81_0/libs/pool/doc/html/boost_pool/pool/pooling.html
 * */
class SimpleSegregatedStorage {
public:
  SimpleSegregatedStorage() : free_memory(nullptr) {}

protected:
  /*
   * Add a segregated block, and link it to the free_memory.
   * */
  void add_block(void *const block, const std::size_t &size,
                 const std::size_t &partition_size) {
    free_memory = segregate(block, size, partition_size, free_memory);
  }

  /*
   * Malloc method of the simple segregated storage.
   * */
  void *memory_pool_malloc();

  /*
   * Free method of the simple segregated storage.
   * */
  void memory_pool_free(void *chunk);

  /*
   * Establish a link with the next node.
   * It's a very tricky idea to store the address content by pointed address.
   * [hint] A bug here for very small partition size (e.g. <= 5 bytes).
   * */
  void *&next_of(void *const ptr) { return *(static_cast<void **>(ptr)); }

  /*
   * Find the previous node by iterating from back to begin.
   * */
  void *find_prev(void *ptr);

	/*
   * The free list, that points to the first trunk.
   * Should be nullptr if the memory_pool_free list is empty.
   * */
	void *free_memory;

 private:
	/*
   * Segregate block into chunks
   * */
	void *segregate(void *block, const std::size_t &total_size,
	                const std::size_t &partition_size, void *end);

  /*
   * Check if the memory list is empty.
   * */
  bool empty() { return (free_memory == nullptr); }
};

void *SimpleSegregatedStorage::segregate(void *block,
                                         const std::size_t &total_size,
                                         const std::size_t &partition_size,
                                         void *end) {
  // get pointer to the last valid chunk
  // last_chunk == block + partition_size * i
  char *last_chunk =
      static_cast<char *>(block) +
      ((total_size - partition_size) / partition_size) * partition_size;

  // set the last valid chunk to the end
  next_of(last_chunk) = end;

  // cannot split into pieces
  if (last_chunk == block) {
    return block;
  }

  // iterate backwards, building a singly-linked list of pointers
  for (char *iter = last_chunk - partition_size; iter != block;
       last_chunk = iter, iter -= partition_size)
    next_of(iter) = last_chunk;

  // point the first pointer
  next_of(block) = last_chunk;

  return block;
}

void *SimpleSegregatedStorage::memory_pool_malloc() {
  void *const ret = free_memory;
  // increase the "free_memory" pointer to point to the next chunk
  free_memory = next_of(free_memory);
  return ret;
}

void SimpleSegregatedStorage::memory_pool_free(void *chunk) {
  // reduce the "free_memory" pointer to the previous node
  next_of(chunk) = free_memory;
  free_memory = chunk;
}

void *SimpleSegregatedStorage::find_prev(void *ptr) {
  if (free_memory == nullptr || std::greater_equal<>()(free_memory, ptr))
    return nullptr;

  void *iter = free_memory;
  while (true) {
    if (next_of(iter) == nullptr || std::greater_equal<>()(next_of(iter), ptr))
      return iter;

    iter = next_of(iter);
  }
}
} // namespace memory_pool

#endif // SIMPLE_SEGREGATED_STORAGE_H