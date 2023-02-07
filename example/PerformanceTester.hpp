/*
 * @author: Pei Mu
 * @description: Data structure of memory blocks
 * @data: 2nd Feb 2023
 * */

#ifndef PERFORMANCE_TESTER_H
#define PERFORMANCE_TESTER_H

#include "ExampleClasses.h"
#include "MemoryPool.hpp"
#include <vector>
#include <unistd.h>

typedef struct timespec timespec;

timespec diff(timespec start, timespec end);
void printTimeSpec(timespec t, const char* prefix);
timespec tic( );
timespec toc(timespec* start_time, const char* prefix );

template <typename element_type, std::size_t N=3>
class PerformanceTester {
 public:
	PerformanceTester() = default;
	~PerformanceTester() = default;

	void test() {
		std::vector<element_type *> ele_mp_vec;
		timespec timer = tic();
		auto mp = memory_pool::MemoryPool<element_type>(g_ChunkNum, g_MaxNumberOfObjectsInPool);
		for (std::size_t chunk_id = 0; chunk_id < g_ChunkNum; chunk_id++) {
			auto memory_chunk = mp.construct();
			ele_mp_vec.emplace_back(memory_chunk);
			// do something with the new allocated memory
		}
		for (auto &ele : ele_mp_vec) {
			mp.destroy(ele);
		}
		auto mp_time = toc(&timer, "computation delay of memory pool");

		std::vector<void *> ele_default_vec;
		for (std::size_t chunk_id = 0; chunk_id < g_ChunkNum; chunk_id++) {
			if constexpr (std::is_default_constructible<element_type>::value) {
				auto memory = new element_type();
				ele_default_vec.emplace_back(memory);
			} else {
				auto memory = new element_type(N);
				ele_default_vec.emplace_back(memory);
			}
			// do something with the new allocated memory
		}
		for (auto &ele : ele_default_vec) {
			memory_pool::destroy_element(ele);
		}
		auto default_time = toc(&timer, "computation delay of system new/delete");
		long time_diff = default_time.tv_nsec - mp_time.tv_nsec;
		long system_time = default_time.tv_nsec;
		double speed_up = (double)time_diff / (double)system_time * 100;
		std::cout << typeid(element_type).name() << " speed up: " << speed_up << "%" << std::endl;
	}
};

#endif //PERFORMANCE_TESTER_H
