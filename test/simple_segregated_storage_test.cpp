/*
 * @author: Pei Mu
 * @description: basic GTest of memory pool
 * @data: 30th Jan 2023
 * */

#include <gtest/gtest.h>
#include "simple_segregated_storage.hpp"

#define BLOCK_SIZE 1000
#define PARTITION_SIZE 33

class SimpleSegregatedStorageTester :
	public arm_exercise::SimpleSegregatedStorage<std::size_t> {
 public:
	void *test_add_block(void *block,
	                    std::size_t total_size,
	                    std::size_t partition_size) {
		add_block(block, total_size, partition_size, nullptr);
		return get_free_memory();
	}

	void *test_next_chunk(void *trunk) {
		return next_of(trunk);
	}

	void *test_prev_chunk(void *trunk) {
		return find_prev(trunk);
	}

	void *test_malloc() {
		return malloc();
	}

	void test_free(void *trunk) {
		free(trunk);
	}

	void *test_get_free_memory() {
		return get_free_memory();
	}

 private:
//	void set_partition_size(const std::size_t size) {
//	 if (allocated_blocks.empty())
//		 total_size = size;
//	 else
//		 assert(total_size == size);
// }

	std::vector<std::pair<void *, std::size_t>> allocated_blocks;
//  std::size_t total_size{};
	std::set<void *> allocated_chunks;
};

TEST(BasicMemoryPoolTest, TestAddBlock) {
	auto sss = SimpleSegregatedStorageTester();
	const std::size_t block_size = BLOCK_SIZE;
	const std::size_t partition_size = PARTITION_SIZE;
	auto block = malloc(block_size);
	auto free_memory = sss.test_add_block(block, block_size, partition_size);
	EXPECT_EQ(block, free_memory);

	auto cur = sss.test_next_chunk(free_memory);
	EXPECT_EQ(sss.test_prev_chunk(cur), free_memory);
	auto last = free_memory;
	for (std::size_t chunk_num = 1; chunk_num < block_size / partition_size; chunk_num++) {
		EXPECT_EQ(static_cast<char *>(last) + partition_size, cur);
		EXPECT_LE(static_cast<char *>(cur) + partition_size, static_cast<char *>(block) + block_size);
		last = cur;
		cur = sss.test_next_chunk(cur);
	}
}

TEST(BasicMemoryPoolTest, TestMallocFree) {
	auto sss = SimpleSegregatedStorageTester();
	const std::size_t block_size = BLOCK_SIZE;
	const std::size_t partition_size = PARTITION_SIZE;
	auto block = malloc(block_size);
	sss.test_add_block(block, block_size, partition_size);

	// test malloc
	auto first_trunk = sss.test_malloc();
	EXPECT_EQ(first_trunk, block);
	auto second_trunk = sss.test_malloc();
	EXPECT_EQ(second_trunk, static_cast<char *>(block) + partition_size);

	// test free
	sss.test_free(first_trunk);
	EXPECT_EQ(sss.test_get_free_memory(), block);

	// test malloc again
	auto third_trunk = sss.test_malloc();
	EXPECT_EQ(third_trunk, block);
	EXPECT_EQ(sss.test_next_chunk(third_trunk), static_cast<char *>(block) + partition_size*2);
}
