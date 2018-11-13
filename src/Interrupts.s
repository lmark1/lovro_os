.section .text

.extern _ZN16InterruptManager22handleInterruptRequestEhj



int_bottom:

    pusha           // Push all general purpose registers
    pushl %ds       // Push data segment
    push %es
    push %fs
    push %gs

    pushl %esp
    push (interruptNumber)
    call _ZN16InterruptManager22handleInterruptRequestEhj
    movl %eax, %esp

.data:
    interruptNumber: .byte 0
