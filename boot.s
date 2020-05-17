/* Multiboot header values */
.set ALIGN,    1<<0		/* bit 1: align loaded boot modules on page (4kb) boundaries */
.set MEMINFO,  1<<1		/* bit 2: multiboot provides memory map */
.set FLAGS,    ALIGN | MEMINFO	/* Flag field containing above 2 options */
.set MAGIC,    0x1BADB002	/* Magic number to indicate multiboot is used */
.set CHECKSUM, -(MAGIC + FLAGS) /* Checksum - check multiboot spec */

/* Declare multiboot header */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM	

/* 
Create 16KB stack in its own section so that it can be declared nobits
which reduces size of kernel as it does not contain an uninit stack.
*/
.section .bss
.align 16
stack_bottom:
.skip 16384 	# 16 KiB
stack_top:

/* _start marks the kernel entrypoint (bootloader will jump here) */
.section .text
.global _start
.type _start, @function
_start:
	# Stack pointer set-up so C programs can operate */
	mov $stack_top, %esp

	# Call kernel main (stack must be 16-byte aligned upon calling kernel)
	call kmain

	# Kernel should never return, but if it does, put computer into an infinite loop
	cli	# Disable most interrupts
1:	hlt	# Wait until interrupt (halts computer until a unmaskable interrupt is signaled)
	jmp 1b	# Loop back to halting (b specifies last "1" label)

# Set size of _start (current byte location "." minus _start symbol location) for debugging
.size _start, . - _start

