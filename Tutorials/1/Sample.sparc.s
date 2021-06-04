	.file	"Sample.c"
	.global global
	.section	".bss"
	.align 4
	.type	global, #object
	.size	global, 4
global:
	.skip 4
	.section	".text"
	.align 4
	.global function
	.type	function, #function
	.proc	04
function:
	save	%sp, -104, %sp
	st	%i0, [%fp+68]
	ld	[%fp+68], %g1
	add	%g1, 123, %g1
	st	%g1, [%fp-4]
	ld	[%fp-4], %g1
	add	%g1, 456, %g2
	sethi	%hi(global), %g1
	or	%g1, %lo(global), %g1
	st	%g2, [%g1]
	ld	[%fp-4], %g1
	mov	%g1, %i0
	return	%i7+8
	 nop
	.size	function, .-function
	.align 4
	.global main
	.type	main, #function
	.proc	04
main:
	save	%sp, -104, %sp
	mov	999, %o0
	call	function, 0
	 nop
	st	%o0, [%fp-4]
	mov	%g1, %i0
	return	%i7+8
	 nop
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.4"
