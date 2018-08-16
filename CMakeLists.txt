cmake_minimum_required(VERSION 3.12)
project(test)

set(CMAKE_CXX_STANDARD 14)
add_definitions(-DWORK_TEST_EXECUTABLE)
add_executable(test main.cpp DownloadTest.cpp DownloadTest.h node_head.h)

target_link_libraries(test libuv.a)