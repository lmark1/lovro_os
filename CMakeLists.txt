cmake_minimum_required(VERSION 2.6)
project(lovro_os)

# Set variables for include and source dir
SET(INCLUDE_DIR "include")
SET(SRC_DIR "src")
SET(BUILD_DIR "build")

# We want to push this option to the linker, therefore -Wl (with linker)
SET(CUSTOM_LINKER_FLAGS "-Wl,-melf_i386")
SET(CUSTOM_LINKER_DIR "${CMAKE_SOURCE_DIR}/linker.ld")
SET(ASM_COMPILE_FLAGS "--32")

SET(CPP_COMPILE_FLAGS 
    "-m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CPP_COMPILE_FLAGS}")

# Enable assembler language
enable_language(ASM-ATT)

# Include header files
include_directories("${INCLUDE_DIR}")

# Generate a STATIC object file loader.o
add_library(loader OBJECT ${SRC_DIR}/loader.s)
target_compile_options(loader PRIVATE "${ASM_COMPILE_FLAGS}")

# Generate a STATIC object file kernel.o
add_library(kernel OBJECT ${SRC_DIR}/kernel.cpp)

# Generate a STATIC object file gdt.o
add_library(gdt OBJECT ${SRC_DIR}/GlobalDescTable.cpp)

# Set a different linker - append appropriate flags to exe linker
set(CMAKE_EXE_LINKER_FLAGS 
    "${CMAKE_EXE_LINKER_FLAGS} -T ${CUSTOM_LINKER_DIR} ${CUSTOM_LINKER_FLAGS}")
    
# Build binary binary file from 2 object files - hack
add_executable(kernel.bin 
    $<TARGET_OBJECTS:loader> 
    $<TARGET_OBJECTS:kernel>
    $<TARGET_OBJECTS:gdt>)
    
# Make ISO file directory tree
SET(GRUB_DIRECTORY "${CMAKE_SOURCE_DIR}/iso/boot/grub") 
SET(BOOT_DIRECTORY "${CMAKE_SOURCE_DIR}/iso/boot")
SET(GRUB_CONFIG_FILE "grub.cfg")
file(MAKE_DIRECTORY "${GRUB_DIRECTORY}")

# Copy kernel.bin file to boot directory
install(
    TARGETS kernel.bin RUNTIME 
    DESTINATION "${BOOT_DIRECTORY}")

# Initialize grub.cfg
file(WRITE "${BUILD_DIR}/${GRUB_CONFIG_FILE}" "set timeout=0\n")
file(APPEND "${BUILD_DIR}/${GRUB_CONFIG_FILE}" "set default=0\n\n")
file(APPEND "${BUILD_DIR}/${GRUB_CONFIG_FILE}" "menuentry \"My Operating System\" {\n")
file(APPEND "${BUILD_DIR}/${GRUB_CONFIG_FILE}" "  multiboot /boot/kernel.bin\n")
file(APPEND "${BUILD_DIR}/${GRUB_CONFIG_FILE}" "  boot\n")
file(APPEND "${BUILD_DIR}/${GRUB_CONFIG_FILE}" "}")

# Copy grub.cfg file to grub directory
install(
    FILES "${BUILD_DIR}/${GRUB_CONFIG_FILE}"
    DESTINATION "${GRUB_DIRECTORY}") 

# Execute grub-mkrescue
install(
    CODE 
    "execute_process(COMMAND grub-mkrescue 
        --output=${CMAKE_SOURCE_DIR}/build/kernel.iso 
        ${CMAKE_SOURCE_DIR}/iso)")