/*
 * @author: Pei Mu
 * @description: basic GTest of memory pool
 * @data: 30th Jan 2023
 * */

#include <gtest/gtest.h>
#include "MemoryPool.hpp"

TEST(MemoryPoolTest, TestCreatePool) {
	auto mp = arm_exercise::MemoryPool<std::size_t>();
}
