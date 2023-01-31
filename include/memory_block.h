/*
 * @author: Pei Mu
 * @description: Data structure of memory blocks
 * @data: 30th Jan 2023
 * */

#ifndef MEMORY_BLOCK_H
#define MEMORY_BLOCK_H

#include <numeric>

namespace arm_exercise {
	template <typename SizeType>
	class MemoryBlock {
	 public:
		MemoryBlock(char * const ptr, const SizeType& size) : ptr(ptr), size(size) {}

	 protected:
		char * begin() {
			return ptr;
		}

		char * end() {
			return ptr_next_ptr();
		}

		bool valid() const {
			return (begin() != 0);
		}

		SizeType total_size() const {
			return size;
		}

		SizeType element_size() const {
			return static_cast<SizeType>(size - sizeof(SizeType)
				- std::lcm(sizeof(SizeType), sizeof(void *)));
		}

		SizeType &next_size() const {
			return *(static_cast<SizeType *>(static_cast<void *>(ptr_next_size())));
		}

		char* &next_ptr() const {
			return *(static_cast<char **>(static_cast<void *>(ptr_next_ptr())));
		}

		MemoryBlock next() const {
			return MemoryBlock<SizeType>(next_ptr(), next_size());
		}

		void next(const MemoryBlock &arg) const {
			next_ptr() = arg.begin();
			next_size() = arg.total_size();
		}

	 private:
		char *ptr_next_size() const {
			return (ptr + size - sizeof(SizeType));
		}

		char *ptr_next_ptr() const {
			return (ptr_next_size() - std::lcm(sizeof(SizeType), sizeof(void *)));
		}

		char *ptr;
		SizeType size;
	};
}

#endif // MEMORY_BLOCK_H
