#0) Configure project directory settings
cmake_minimum_required(VERSION 3.31)
project(CompuTron)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#1) Add the test executable
add_executable(computron_main src/main.cpp src/computron.cpp p1.txt)
add_executable(computron_test tests/test.cpp src/computron.cpp p1.txt)

#2) Include directories for the test target
target_include_directories(computron_main PRIVATE $(PROJECT_SOURCE_DIR)/include)
target_include_directories(computron_test PRIVATE ${PROJECT_SOURCE_DIR}/include)

#3) Enable testing
enable_testing()
