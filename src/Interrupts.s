.set IRQ_BASE, 0x20

# Start code section
.section .text

# InterruptManager::handleInterrupt method will be defined externally
.extern _ZN16InterruptManager15handleInterruptEhj

# Global function: InterruptManager::IgnoreInterruptRequest
.global _ZN16InterruptManager22IgnoreInterruptRequestEv


# Define macro for handling interrupt requests
# num - interrupt number
.macro HandleInterruptRequest num

    # Extern global function: InterruptManager::HandleInterruptRequest
    .global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev

    # Define: InterruptManager::HandleInterruptRequest
    _ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
        movb $\num + IRQ_BASE, (interruptNumber)
        jmp int_bottom
.endm

# Define macro for handling exceptions
# num - exception number
.macro HandleException num

    # Extern global function: InterruptManager::HandleException
    .global _ZN16InterruptManager15HandleException\num\()Ev

    # Define: InterruptManger::HandleException
    _ZN16InterruptManager15HandleException\num\()Ev:
        movb $\num, (interruptNumber)
        jmp int_bottom
.endm

# Make calls to both macros
HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

int_bottom:

    # Push all segment registers to stack
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    # Push InterruptManager::handleInterrupt function args to stack, and call
    pushl %esp
    push (interruptNumber)
    call _ZN16InterruptManager15handleInterruptEhj

    # Restore the stack pointer
    addl %esp, 6
    movl %esp, %eax

    # Pop all segment registers to stack
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

_ZN16InterruptManager22IgnoreInterruptRequestEv:
    iret


.data
    interruptNumber: .byte 0
