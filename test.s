	.text
	.file	"swi2else"
	.globl	test_f                  # -- Begin function test_f
	.p2align	4, 0x90
	.type	test_f,@function
test_f:                                 # @test_f
	.cfi_startproc
# %bb.0:                                # %entry
	movsd	%xmm0, -16(%rsp)
	movq	$0, -8(%rsp)
	movl	$4, -20(%rsp)
	movl	$8, %eax
	retq
.Lfunc_end0:
	.size	test_f, .Lfunc_end0-test_f
	.cfi_endproc
                                        # -- End function

	.section	".note.GNU-stack","",@progbits
