.set IRQ_BASE, 0x20
.section .text
.extern _ZN16InterruptManager15HandleInterruptEhj

.macro HandleException num
global _ZN16InterruptManager19HandleException\num\()Ev
_ZN16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptNumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptNumber)
    jmp int_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x0C

int_bottom:
	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs
	pushl %esp
	push (interruptNumber)
	call _ZN16InterruptManager15HandleInterruptEhj
	# addl $5, %esp
	movl %eax, %esp
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
.global _ZN16InterruptManager15InterruptIgnoreEv
_ZN16InterruptManager15InterruptIgnoreEv:
    iret

.data
	interruptNumber: .byte 0
