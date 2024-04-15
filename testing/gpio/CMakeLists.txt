cmake_minimum_required(VERSION 3.2)

project(main
        VERSION 1.0.0
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(JetsonGPIO)

add_executable(main main.cpp)

target_link_libraries(main JetsonGPIO::JetsonGPIO)
