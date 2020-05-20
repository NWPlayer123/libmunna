cmake_minimum_required (VERSION 3.8)
project(munna)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -pthread -fdiagnostics-color=always") #-Wextra
set(CMAKE_CXX_FLAGS_DEBUG "-Og")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS "-Wall -pthread -fdiagnostics-color=always") #-Wextra
set(CMAKE_C_FLAGS_DEBUG "-Og")
set(CMAKE_C_FLAGS_RELEASE "-O2")

include_directories(include)

set(SOURCES
    src/munna/munna_file.cpp
    src/munna/filetypes/munna_yaz0.cpp
)

add_library(munna STATIC ${SOURCES})
add_executable(munna_testbench src/main.cpp)
target_link_libraries(munna_testbench munna)