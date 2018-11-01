# Set magic number so the bootloader recognizes this as a OS kernel
.set MAGIC, 0x1badb002
.set FLAGS

# Define a new section called .text
.section .text

# Expect an outiside function "kernelMain"
.extern kernelMain

# Global makes symbol "loader" visible to linker
.global loader

# Main loader program
# First we need to put stack pointer to kernel stack adress
# When we jump into kernelMain we are not getting out
loader:
    mov $kernel_stack, %esp
    call kernelMain

# Infinite loop
_stop:
    cli
    hlt
    jmp _stop

# Define a new section called .bss
.section .bss

# Kernel stack pointer is filling from the bottom - up so we need free space here
.space 2*1024*1024

# Part of the memory for the kernel stack
kernel_stack:
