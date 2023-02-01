/*
 * @author: Pei Mu
 * @description: basic GTest of memory pool
 * @data: 30th Jan 2023
 * */

#include <gtest/gtest.h>
#include "MemoryPool.hpp"

template <typename element_type>
class MemoryPoolTester :
	public arm_exercise::MemoryPool<element_type> {
 public:
	explicit MemoryPoolTester(std::size_t chunks_num_val = 32,
	                          std::size_t max_chunks_val = 0) :
														arm_exercise::MemoryPool<element_type>(chunks_num_val,
																															 max_chunks_val) {}

	arm_exercise::MemoryBlock get_memory_blocks() {
		return this->memory_blocks;
	}

	std::size_t get_requested_size() {
		return this->requested_size;
	}

	std::size_t get_chunks_num() {
		return this->chunk_num;
	}

	std::size_t get_max_chunk_num() {
		return this->max_chunk_num;
	}

	void *get_next_chunk(void *const chunk) {
		return this->next_of(chunk);
	}

	bool test_purge_memory() {
		return this->purge_memory();
	}
};

TEST(MemoryPoolTest, TestDefaultSizeT) {
	// test memory pool construction
	auto mp = MemoryPoolTester<size_t>();
	auto memory_chunk = mp.memory_pool_malloc();
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the default max chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 32*2);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), sizeof(std::size_t));

	// test malloc
	auto second_chunk = mp.memory_pool_malloc();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	EXPECT_EQ(second_chunk, static_cast<std::size_t *>(mp.get_memory_blocks().begin()) + sizeof(std::size_t));

	// test free
	mp.memory_pool_free(memory_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(),
						second_chunk - sizeof(std::size_t));

	// malloc again
	auto third_trunk = mp.memory_pool_malloc();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	EXPECT_EQ(mp.get_next_chunk(third_trunk),
	          static_cast<std::size_t *>(mp.get_memory_blocks().begin()) + sizeof(std::size_t)*2);

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}
