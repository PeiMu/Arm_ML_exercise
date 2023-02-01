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
	template <typename SizeType>
	class MemoryPool : protected SimpleSegregatedStorage<SizeType> {
	 public:
		explicit MemoryPool(const SizeType& requested_size_val = sizeof(SizeType),
												const SizeType& chunks_num_val = 32,
												const SizeType& max_chunks_val = 0) :
			list(nullptr, 0),
			requested_size(requested_size_val) {
			set_next_size(chunks_num_val);
			set_max_size(max_chunks_val);
		}

		~MemoryPool() {
			purge_memory();
		}

		void *memory_pool_malloc() {
			if (this->free_memory != nullptr)
				return SimpleSegregatedStorage<SizeType>::memory_pool_malloc();
			return malloc_need_resize();
		}

		void memory_pool_free(void *const chunk) {
			SimpleSegregatedStorage<SizeType>::memory_pool_free(chunk);
		}

		bool shrink_memory();

	 protected:
		void set_next_size(const SizeType& next_size_val) {
			chunks_num = start_size = std::min(next_size_val, max_chunks());
		}

		void set_max_size(const SizeType& max_size_val) {
			max_chunks_num = std::min(max_size_val, max_chunks());
		}

		bool purge_memory();

		bool is_from(void *const chunk, char *const i, const SizeType size) {
			std::less_equal<> lt_eq;
			std::less<> lt;
			return (lt_eq(i, chunk) && lt(chunk, i + size));
		}

		MemoryBlock<SizeType> list;
		const SizeType requested_size;
		SizeType chunks_num;
		SizeType start_size;
		SizeType max_chunks_num;

	 private:
		SizeType alloc_size() const {
			SizeType s = std::max(requested_size, min_alloc_size);
			SizeType rem = s % min_align;
			if (rem)
				s += min_align - rem;
			assert(s >= min_alloc_size);
			assert(s % min_align == 0);
			return s;
		}

		SizeType max_chunks() const {
			SizeType block_size = std::lcm(sizeof(SizeType), sizeof(void *)) + sizeof(SizeType);
			return (std::numeric_limits<SizeType>::max() - block_size) / alloc_size();
		}

		void *malloc_need_resize();

		const SizeType min_alloc_size = std::lcm(sizeof(void *), sizeof(SizeType));
		const SizeType min_align = std::lcm(std::alignment_of<void *>::value, std::alignment_of<SizeType>::value);
	};

	template<typename SizeType>
	void *MemoryPool<SizeType>::malloc_need_resize() {
		SizeType partition_size = alloc_size();
		auto block_size = static_cast<SizeType>(chunks_num * partition_size +
		                                        std::lcm(sizeof(SizeType), sizeof(void *)) + sizeof(SizeType));
		char *ptr = (char *)malloc(block_size);
		if (ptr == nullptr) {
			if (chunks_num > 4) {
				chunks_num >>= 1;
				partition_size = alloc_size();
				block_size = static_cast<SizeType>(chunks_num * partition_size +
				                                   std::lcm(sizeof(SizeType), sizeof(void *)) + sizeof(SizeType));
				ptr = (char *)malloc(block_size);
			}
			if (ptr == nullptr)
				return nullptr;
		}

		MemoryBlock<SizeType> node(ptr, block_size);

		if (!max_chunks_num)
			set_next_size(chunks_num << 1);
		else if (chunks_num * partition_size / requested_size < max_chunks_num)
			set_next_size(std::min(chunks_num << 1, max_chunks_num * requested_size / partition_size));

		this->add_block(node.begin(), node.element_size(), partition_size);

		node.next(list);
		list = node;

		return this->memory_pool_malloc();
	}

	template<typename SizeType>
	bool MemoryPool<SizeType>::purge_memory() {
		MemoryBlock<SizeType> iter = list;
		if (!iter.valid())
			return false;

		do {
			auto next = iter.next();
			memory_pool_free(iter.begin());
			iter = next;
		} while (iter.valid());

		list.invalidate();
		this->free_memory = 0;
		chunks_num = start_size;
		return true;
	}

	template<typename SizeType>
	bool MemoryPool<SizeType>::shrink_memory() {
		bool ret = false;
		MemoryBlock<SizeType> ptr = list;
		MemoryBlock<SizeType> prev;

		void *free_p = this->free_memory;
		void *prev_free_p = nullptr;

		const SizeType partition_size = alloc_size();

		while (ptr.valid()) {
			if (free_p == nullptr)
				break;

			bool all_chunk_free = true;

			void *saved_free = free_p;
			for (char *i = ptr.begin(); i != ptr.end(); i += partition_size) {
				if (i != free_p) {
					all_chunk_free = false;
					free_p = saved_free;
					break;
				}

				free_p = this->next_of(free_p);
			}

			auto next = ptr.next();
			if (!all_chunk_free) {
				if (is_from(free_p, ptr.begin(), ptr.element_size())) {
					std::less<> lt;
					void *const end = ptr.end();
					do {
						prev_free_p = free_p;
						free_p = this->next_of(free_p);
					} while (free_p && lt(free_p, end));
				}

				prev = ptr;
			} else {
				if (prev.valid())
					prev.next(next);
				else
					list = next;

				if (prev_free_p != nullptr)
					this->next_of(prev_free_p) = free_p;
				else
					this->free_memory = free_p;

				memory_pool_free(ptr.begin());
				ret = true;
			}

			ptr = next;
		}

		chunks_num = start_size;
		return ret;
	}
}

#endif // MEMORY_POOL_H
