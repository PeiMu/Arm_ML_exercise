/*
 * @author: Pei Mu
 * @description: Implementation of simple segregated storage algorithm
 * @data: 30th Jan 2023
 * */

#ifndef SIMPLE_SEGREGATED_STORAGE_H
#define SIMPLE_SEGREGATED_STORAGE_H

namespace arm_exercise {
	/*
	 *
	 * */
	template <typename SizeType>
	class SimpleSegregatedStorage {
	 public:
		SimpleSegregatedStorage() : free_list(nullptr) {}

	 protected:
		/*
		 * Segregate block into chunks and merge it into free_list
		 * */
		void add_block(void *block,
									 SizeType size,
									 SizeType partition_size);

		void *malloc();

		void free(void *chunk);

		/*
		 * The free list, that points to the first trunk.
		 * Should be nullptr if the free list is empty.
		 * */
		void *free_list;

	 private:
		static void* &next_of(void *const ptr) {
			return *(static_cast<void**>(ptr));
		}
	};
}

#endif // SIMPLE_SEGREGATED_STORAGE_H