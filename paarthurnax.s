.LC0:
	.string "%ld\n"
.LC1:
	.string "%ld"
	.globl main
main:
	pushq	%rbp
	movq	%rsp, %rbp
	call	main
