/*
 * @author: Pei Mu
 * @description: basic GTest of memory pool
 * @data: 30th Jan 2023
 * */

#include <gtest/gtest.h>
#include "MemoryPool.hpp"
#include "ExampleClasses.h"

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
	auto mp = MemoryPoolTester<std::size_t>();
	auto memory_chunk = mp.memory_pool_malloc();
	const int partition_size = sizeof(std::size_t);
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the temporary chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 64);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), partition_size);

	// test malloc
	auto second_chunk = mp.memory_pool_malloc();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	void *address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size;
	EXPECT_EQ(second_chunk, static_cast<std::size_t *>(address));

	// test free
	mp.memory_pool_free(memory_chunk);
	address = static_cast<void *>(second_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(), static_cast<char *>(address) - partition_size);

	// malloc again
	auto third_trunk = mp.memory_pool_malloc();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size*2;
	EXPECT_EQ(mp.get_next_chunk(third_trunk), static_cast<std::size_t *>(address));

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}

TEST(MemoryPoolTest, TestFloat) {
	// test memory pool construction
	auto mp = MemoryPoolTester<float>(10, 1000);
	auto memory_chunk = mp.memory_pool_malloc();
	const int partition_size = std::lcm(sizeof(float), sizeof(void *));
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the temporary chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 20);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), sizeof(float));

	// test malloc
	auto second_chunk = mp.memory_pool_malloc();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	void *address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size;
	EXPECT_EQ(second_chunk, static_cast<float *>(address));

	// test free
	mp.memory_pool_free(memory_chunk);
	address = static_cast<void *>(second_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(), static_cast<char *>(address) - partition_size);

	// malloc again
	auto third_trunk = mp.memory_pool_malloc();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size*2;
	EXPECT_EQ(mp.get_next_chunk(third_trunk), static_cast<float *>(address));

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}

TEST(MemoryPoolTest, TestByteType) {
	// test memory pool construction
	auto mp = MemoryPoolTester<ByteType>(10, 1000);
	auto memory_chunk = mp.memory_pool_malloc();
	const int partition_size = std::lcm(sizeof(ByteType), sizeof(void *));
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the temporary chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 20);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), sizeof(ByteType));

	// test malloc
	auto second_chunk = mp.memory_pool_malloc();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	void *address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size;
	EXPECT_EQ(second_chunk, static_cast<ByteType *>(address));

	// test free
	mp.memory_pool_free(memory_chunk);
	address = static_cast<void *>(second_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(), static_cast<char *>(address) - partition_size);

	// malloc again
	auto third_trunk = mp.memory_pool_malloc();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size*2;
	EXPECT_EQ(mp.get_next_chunk(third_trunk), static_cast<ByteType *>(address));

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}

TEST(MemoryPoolTest, TestPointerType) {
	// test memory pool construction
	auto mp = MemoryPoolTester<PointerType>(10, 1000);
	auto memory_chunk = mp.memory_pool_malloc();
	const int partition_size = std::lcm(sizeof(PointerType), sizeof(void *));
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the temporary chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 20);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), sizeof(PointerType));

	// test malloc
	auto second_chunk = mp.memory_pool_malloc();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	void *address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size;
	EXPECT_EQ(second_chunk, static_cast<PointerType *>(address));

	// test free
	mp.memory_pool_free(memory_chunk);
	address = static_cast<void *>(second_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(), static_cast<char *>(address) - partition_size);

	// malloc again
	auto third_trunk = mp.memory_pool_malloc();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size*2;
	EXPECT_EQ(mp.get_next_chunk(third_trunk), static_cast<PointerType *>(address));

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}

TEST(MemoryPoolTest, TestCharArray) {
	// test memory pool construction
	auto mp = MemoryPoolTester<FixedStringType>(10, 1000);
	auto memory_chunk = mp.memory_pool_malloc();
	const int partition_size = std::lcm(sizeof(FixedStringType), sizeof(void *));
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the temporary chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 20);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), sizeof(FixedStringType));

	// test malloc
	auto second_chunk = mp.memory_pool_malloc();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	void *address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size;
	EXPECT_EQ(second_chunk, static_cast<FixedStringType *>(address));

	// test free
	mp.memory_pool_free(memory_chunk);
	address = static_cast<void *>(second_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(), static_cast<char *>(address) - partition_size);

	// malloc again
	auto third_trunk = mp.memory_pool_malloc();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size*2;
	EXPECT_EQ(mp.get_next_chunk(third_trunk), static_cast<FixedStringType *>(address));

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}

TEST(MemoryPoolTest, TestStructure) {
	// test memory pool construction
	auto mp = MemoryPoolTester<Point>(10, 1000);
	auto memory_chunk = mp.memory_pool_malloc();
	const int partition_size = std::lcm(sizeof(Point), sizeof(void *));
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the temporary chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 20);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), sizeof(Point));

	// test malloc
	auto second_chunk = mp.memory_pool_malloc();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	void *address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size;
	EXPECT_EQ(second_chunk, static_cast<Point *>(address));

	// test free
	mp.memory_pool_free(memory_chunk);
	address = static_cast<void *>(second_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(), static_cast<char *>(address) - partition_size);

	// malloc again
	auto third_trunk = mp.memory_pool_malloc();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size*2;
	EXPECT_EQ(mp.get_next_chunk(third_trunk), static_cast<Point *>(address));

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}

TEST(MemoryPoolTest, TestBaseClass) {
	// test memory pool construction
	auto mp = MemoryPoolTester<Base1>(10, 1000);
	auto memory_chunk = mp.construct();
	const int partition_size = std::lcm(sizeof(Base1), sizeof(void *));
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the temporary chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 20);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), sizeof(Base1));

	// test malloc
	auto second_chunk = mp.construct();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	void *address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size;
	EXPECT_EQ(second_chunk, static_cast<Base1 *>(address));

	// test free
	mp.destroy(memory_chunk);
	address = static_cast<void *>(second_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(), static_cast<char *>(address) - partition_size);

	// malloc again
	auto third_trunk = mp.construct();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size*2;
	EXPECT_EQ(mp.get_next_chunk(third_trunk), static_cast<Base1 *>(address));

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}

TEST(MemoryPoolTest, TestDerivedClass) {
	// test memory pool construction
	auto mp = MemoryPoolTester<Derived>(10, 1000);
	auto memory_chunk = mp.construct();
	const int partition_size = std::lcm(sizeof(Derived), sizeof(void *));
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the temporary chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 20);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), sizeof(Derived));

	// test malloc
	auto second_chunk = mp.construct();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	void *address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size;
	EXPECT_EQ(second_chunk, static_cast<Derived *>(address));

	// test free
	mp.destroy(memory_chunk);
	address = static_cast<void *>(second_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(), static_cast<char *>(address) - partition_size);

	// malloc again
	auto third_trunk = mp.construct();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size*2;
	EXPECT_EQ(mp.get_next_chunk(third_trunk), static_cast<Derived *>(address));

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}

TEST(MemoryPoolTest, TestNodefaultConstClass) {
	// test memory pool construction
	auto mp = MemoryPoolTester<NoDefaultConstructor>(10, 1000);
	auto memory_chunk = mp.construct();
	const int partition_size = std::lcm(sizeof(NoDefaultConstructor), sizeof(void *));
	EXPECT_EQ(memory_chunk, mp.get_memory_blocks().begin());
	// for the temporary chunk size, it's the double size of the chunk number
	EXPECT_EQ(mp.get_chunks_num(), 20);
	// by default, the requested size is the size of data type
	EXPECT_EQ(mp.get_requested_size(), sizeof(NoDefaultConstructor));

	// test malloc
	auto second_chunk = mp.construct();
	// check the address of next chunk
	EXPECT_EQ(mp.get_next_chunk(memory_chunk), second_chunk);
	void *address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size;
	EXPECT_EQ(second_chunk, static_cast<NoDefaultConstructor *>(address));

	// test free
	mp.destroy(memory_chunk);
	address = static_cast<void *>(second_chunk);
	EXPECT_EQ(mp.get_memory_blocks().begin(), static_cast<char *>(address) - partition_size);

	// malloc again
	auto third_trunk = mp.construct();
	EXPECT_EQ(third_trunk, mp.get_memory_blocks().begin());
	address = static_cast<char *>(mp.get_memory_blocks().begin()) + partition_size*2;
	EXPECT_EQ(mp.get_next_chunk(third_trunk), static_cast<NoDefaultConstructor *>(address));

	// test memory pool destroy
	EXPECT_TRUE(mp.test_purge_memory());
	EXPECT_EQ(mp.get_memory_blocks().begin(), nullptr);
}
