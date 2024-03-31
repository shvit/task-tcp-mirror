cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME ttm)
set(RELEASE_DATE 2024-03-31)
project(${PROJECT_NAME})

set(SOURCE src/ttm.cpp src/Server.cpp)
set(HEADER src/Common.hpp src/Server.hpp)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} ev)