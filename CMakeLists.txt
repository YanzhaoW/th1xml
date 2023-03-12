cmake_minimum_required(VERSION 3.13.4)

project(th1xml VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(ROOT REQUIRED COMPONENTS Hist RIO)

find_package(Boost REQUIRED COMPONENTS program_options)

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cxx)
target_link_libraries(${PROJECT_NAME} support)
