/*
 * @author: Pei Mu
 * @description: Implementation of simple segregated storage algorithm
 * @data: 30th Jan 2023
 * */

#ifndef SIMPLE_SEGREGATED_STORAGE_H
#define SIMPLE_SEGREGATED_STORAGE_H

#include <cassert>

namespace arm_exercise {
	/*
	 *
	 * */
	template <typename SizeType>
	class SimpleSegregatedStorage {
	 public:
		SimpleSegregatedStorage() : free_memory(nullptr) {}

	 protected:
		void add_block(void *const block, const SizeType& size, const SizeType& partition_size) {
			free_memory = segregate(block, size, partition_size, free_memory);
		}

		virtual void *memory_pool_malloc();

		virtual void memory_pool_free(void *chunk);

		/*
		 * Segregate block into chunks and merge it into free_memory
		 * */
		void *segregate(void *block,
		               const SizeType& total_size,
		               const SizeType& partition_size,
		               void *end);

		void* &next_of(void *const ptr) {
			return *(static_cast<void**>(ptr));
		}

		void* find_prev(void *ptr);

		bool empty() {
			return (free_memory == nullptr);
		}

		/*
		 * The free list, that points to the first trunk.
		 * Should be nullptr if the memory_pool_free list is empty.
		 * */
		void *free_memory;
	};

	template<typename SizeType>
	void *SimpleSegregatedStorage<SizeType>::segregate(void *block,
	                                                  const SizeType& total_size,
	                                                  const SizeType& partition_size,
	                                                  void *end) {
		// get pointer to the last valid chunk
		// last_chunk == block + partition_size * i
		char *last_chunk = static_cast<char *>(block)
		                   + ((total_size - partition_size) / partition_size) * partition_size;

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

	template<typename SizeType>
	void *SimpleSegregatedStorage<SizeType>::memory_pool_malloc() {
		void * const ret = free_memory;
		// increase the "free_memory" pointer to point to the next chunk
		free_memory = next_of(free_memory);
		return ret;
	}

	template<typename SizeType>
	void SimpleSegregatedStorage<SizeType>::memory_pool_free(void *chunk) {
		next_of(chunk) = free_memory;
		free_memory = chunk;
	}

	template<typename SizeType>
	void *SimpleSegregatedStorage<SizeType>::find_prev(void *ptr) {
		if (free_memory == nullptr || std::greater_equal<>()(free_memory, ptr))
			return nullptr;

		void *iter = free_memory;
		while (true) {
			if (next_of(iter) == nullptr || std::greater_equal<>()(next_of(iter), ptr))
				return iter;

			iter = next_of(iter);
		}
	}
}

#endif // SIMPLE_SEGREGATED_STORAGE_H