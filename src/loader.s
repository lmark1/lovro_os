# Set magic number so the bootloader recognizes this as a OS kernel
.set MAGIC, 0x1badb002
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

# Put all needed variables in a seperate section
.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

# Define a new section called .text
.section .text

# Expect an outside function "kernelMain" and "callConstructors"
.extern kernelMain
.extern callConstructors

# Global makes symbol "loader" visible to linker
.global loader

# Main loader program
# First we need to put stack pointer to kernel stack adress
# When we jump into kernelMain we are not getting out

# When the bootloader decides this is a kernel...
# It stores information in RAM - "multiboot structure"
# First call constructors
# AX register - pointer to the multiboot structure
# BX register - contains magic number
# Pass that information to kernel main by pushing it on stack
loader:
    mov $kernel_stack, %esp
    call callConstructors
    push %eax
    push %ebx
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
