cmake_minimum_required(VERSION 3.10)
project(lab5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<CONFIG:Debug>" "MultiThreaded$<CONFIG:Release>")
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)

add_executable(${CMAKE_PROJECT_NAME}_exe src/main.cpp src/memory_resource.cpp)
target_include_directories(${CMAKE_PROJECT_NAME}_exe PRIVATE include)

enable_testing()
add_executable(tests test/test_queue.cpp src/memory_resource.cpp)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests PRIVATE gtest_main gtest)
add_test(NAME QueueTests COMMAND tests)
