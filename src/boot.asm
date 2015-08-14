; Declare constants used for creating a multiboot header.
MBALIGN     equ  1<<0                   ; align loaded modules on page boundaries
MEMINFO     equ  1<<1                   ; provide memory map
FLAGS       equ  MBALIGN | MEMINFO      ; this is the Multiboot 'flag' field
MAGIC       equ  0x1BADB002             ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)        ; checksum of above, to prove we are multiboot

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
 

section .bootstrap_stack, nobits
align 4
stack_bottom:
resb 16384
stack_top:
 

section .text
global _start
_start:
	; Welcome to kernel mode! We now have sufficient code for the bootloader to
	; load and run our operating system. It doesn't do anything interesting yet.
	; Perhaps we would like to call printf("Hello, World\n"). You should now
	; realize one of the profound truths about kernel mode: There is nothing
	; there unless you provide it yourself. There is no printf function. There
	; is no <stdio.h> header. If you want a function, you will have to code it
	; yourself. And that is one of the best things about kernel development:
	; you get to make the entire system yourself. You have absolute and complete
	; power over the machine, there are no security restrictions, no safe
	; guards, no debugging mechanisms, there is nothing but what you build.
 
	; By now, you are perhaps tired of assembly language. You realize some
	; things simply cannot be done in C, such as making the multiboot header in
	; the right section and setting up the stack. However, you would like to
	; write the operating system in a higher level language, such as C or C++.
	; To that end, the next task is preparing the processor for execution of
	; such code. C doesn't expect much at this point and we only need to set up
	; a stack. Note that the processor is not fully initialized yet and stuff
	; such as floating point instructions are not available yet.
 
	; To set up a stack, we simply set the esp register to point to the top of
	; our stack (as it grows downwards).
	mov esp, stack_top
 
	; We are now ready to actually execute C code. We cannot embed that in an
	; assembly file, so we'll create a kernel.c file in a moment. In that file,
	; we'll create a C entry point called kernel_main and call it here.
	extern kernel_main
	call kernel_main
 
	; In case the function returns, we'll want to put the computer into an
	; infinite loop. To do that, we use the clear interrupt ('cli') instruction
	; to disable interrupts, the halt instruction ('hlt') to stop the CPU until
	; the next interrupt arrives, and jumping to the halt instruction if it ever
	; continues execution, just to be safe.
	cli
.hang:
	hlt
	jmp .hang