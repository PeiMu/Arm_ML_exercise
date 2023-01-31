/*
 * @author: Pei Mu
 * @description: Memory pool hpp implementation for ARM's interview exercise
 * @data: 30th Jan 2023
 * */

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "memory_block.h"
#include "simple_segregated_storage.hpp"

namespace arm_exercise {
	/*
	 *
	 * */
	template <typename SizeType>
	class MemoryPool : protected SimpleSegregatedStorage <SizeType> {
	 public:
		explicit MemoryPool(const SizeType& requested_size,
												const SizeType& next_size,
												const SizeType& max_size) :
												list(0, 0) {}
	 protected:
		MemoryBlock<SizeType> list;

	 private:

	};
}

#endif // MEMORY_POOL_H
