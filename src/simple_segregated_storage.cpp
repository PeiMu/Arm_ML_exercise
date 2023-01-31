/*
 * @author: Pei Mu
 * @description: Implementation of simple segregated storage algorithm
 * @data: 30th Jan 2023
 * */

#include "simple_segregated_storage.h"

namespace arm_exercise {
	template<typename SizeType>
	void SimpleSegregatedStorage<SizeType>::add_block(void *block,
																				SizeType size,
																				SizeType partition_size) {
		// get pointer to the last valid chunk
		// last_chunk == block + partition_size * i
		char *last_chunk = static_cast<char *>(block)
			+ ((size - partition_size) / partition_size) * partition_size;

		// set the last valid chunk to the end
		next_of(last_chunk) = free_list;

		// iterate backwards, building a singly-linked list of pointers
		for (char *iter = last_chunk - partition_size; iter != block;
				last_chunk = iter, iter -= partition_size)
			next_of(iter) = last_chunk;

		// point the first pointer
		next_of(block) = last_chunk;

		// merge it into free_list
		free_list = block;
	}

	template<typename SizeType>
	void *SimpleSegregatedStorage<SizeType>::malloc() {
		void * const ret = free_list;
		// increase the "free_list" pointer to point to the next chunk
		free_list = next_of(free_list);
		return ret;
	}

	template<typename SizeType>
	void SimpleSegregatedStorage<SizeType>::free(void *chunk) {
		next_of(chunk) = free_list;
		free_list = chunk;
	}
}