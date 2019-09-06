	.text
	.file	"swi2else"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               # -- Begin function func
.LCPI0_0:
	.quad	4607182418800017408     # double 1
	.text
	.globl	func
	.p2align	4, 0x90
	.type	func,@function
func:                                   # @func
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$1, -12(%rsp)
	movabsq	$4614388178203810202, %rax # imm = 0x400999999999999A
	movq	%rax, -8(%rsp)
	movsd	.LCPI0_0(%rip), %xmm1   # xmm1 = mem[0],zero
	jmp	.LBB0_1
	.p2align	4, 0x90
.LBB0_4:                                # %loop2
                                        #   in Loop: Header=BB0_1 Depth=1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -8(%rsp)
	incl	%eax
	movl	%eax, -12(%rsp)
.LBB0_1:                                # %loop1
                                        # =>This Inner Loop Header: Depth=1
	movl	-12(%rsp), %eax
	cmpl	$8, %eax
	xorps	%xmm0, %xmm0
	movapd	%xmm1, %xmm2
	jb	.LBB0_3
# %bb.2:                                # %loop1
                                        #   in Loop: Header=BB0_1 Depth=1
	xorpd	%xmm2, %xmm2
.LBB0_3:                                # %loop1
                                        #   in Loop: Header=BB0_1 Depth=1
	ucomisd	%xmm0, %xmm2
	movsd	-8(%rsp), %xmm0         # xmm0 = mem[0],zero
	jne	.LBB0_4
# %bb.5:                                # %afterloop
	retq
.Lfunc_end0:
	.size	func, .Lfunc_end0-func
	.cfi_endproc
                                        # -- End function

	.section	".note.GNU-stack","",@progbits
