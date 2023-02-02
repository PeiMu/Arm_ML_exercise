/*
 * @author: Pei Mu
 * @description: Data structure of memory blocks
 * @data: 2nd Feb 2023
 * */

#ifndef PERFORMANCE_TESTER_H
#define PERFORMANCE_TESTER_H

#include "ExampleClasses.h"
#include "MemoryPool.hpp"

typedef struct timespec timespec;

timespec diff(timespec start, timespec end);
timespec sum(timespec t1, timespec t2);
void printTimeSpec(timespec t, const char* prefix);
timespec tic( );
timespec toc(timespec* start_time, const char* prefix );

template <typename element_type>
class PerformanceTester {
 public:
	void test() {
		timespec timer = tic();
		auto mp = arm_exercise::MemoryPool<element_type>(g_ChunkNum, g_MaxNumberOfObjectsInPool);
		for (std::size_t chunk_id = 0; chunk_id < g_ChunkNum; chunk_id++) {

		}
		auto mp_time = toc(&timer, "computation delay of memory pool");
	}
};

#endif //PERFORMANCE_TESTER_H
