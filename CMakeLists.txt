cmake_minimum_required(VERSION 3.10)

project(cpp-project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB SOURCES "src/*.cpp")

add_executable(cpp-project ${SOURCES})
target_include_directories(cpp-project PRIVATE include)
