cmake_minimum_required(VERSION 3.24)
project(Programming_exercise_v1_4)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall")

include_directories(include)

add_executable(Programming_exercise_v1_4
        example/Example.cpp)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        memory_pool_test
        test/MemoryPoolTest.cpp)

target_link_libraries(
        memory_pool_test
        GTest::gtest_main
)

add_executable(
        simple_segregated_storage_test
        test/SimpleSegregatedStorageTest.cpp
)

target_link_libraries(
        simple_segregated_storage_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(memory_pool_test)