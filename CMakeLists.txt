cmake_minimum_required(VERSION 2.6)
project(lovro_os)

SET(ASM_COMPILE_FLAGS "--32")
SET(CPP_COMPILE_FLAGS "-m32")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CPP_COMPILE_FLAGS}")
MESSAGE(STATUS "Changing compile flags for CXX: ${CMAKE_CXX_FLAGS}")

# Enable assembler language
enable_language(ASM-ATT)

# Generate a STATIC object file loader.o
add_library(loader STATIC loader.s)
target_compile_options(loader PRIVATE "${ASM_COMPILE_FLAGS}")

# Generate a STATIC object file kernel.o
add_library(kernel STATIC kernel.cpp)

# TODO: Include linker.ld file