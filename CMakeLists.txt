cmake_minimum_required(VERSION 2.6)
project(lovro_os)

# We want to push this option to the linker, therefore -Wl (with linker)
SET(CUSTOM_LINKER_FLAGS "-Wl,-melf_i386")
SET(CUSTOM_LINKER_DIR "${CMAKE_SOURCE_DIR}/linker.ld")
SET(ASM_COMPILE_FLAGS "--32")

SET(CPP_COMPILE_FLAGS 
    "-m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CPP_COMPILE_FLAGS}")

# Enable assembler language
enable_language(ASM-ATT)

# Generate a STATIC object file loader.o
add_library(loader OBJECT loader.s)
target_compile_options(loader PRIVATE "${ASM_COMPILE_FLAGS}")

# Generate a STATIC object file kernel.o
add_library(kernel OBJECT kernel.cpp)

# Set a different linker - append appropriate flags to exe linker
set(CMAKE_EXE_LINKER_FLAGS 
    "${CMAKE_EXE_LINKER_FLAGS} -T ${CUSTOM_LINKER_DIR} ${CUSTOM_LINKER_FLAGS}")
    
# Build binary binary file from 2 object files - hack
add_executable(kernel.bin 
    $<TARGET_OBJECTS:loader> 
    $<TARGET_OBJECTS:kernel>)