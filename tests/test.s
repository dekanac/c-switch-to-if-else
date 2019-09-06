	.text
	.file	"swi2else"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               # -- Begin function func
.LCPI0_0:
	.quad	4607182418800017408     # double 1
.LCPI0_1:
	.quad	-4616189618054758400    # double -1
	.text
	.globl	func
	.p2align	4, 0x90
	.type	func,@function
func:                                   # @func
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$1, -28(%rsp)
	movq	$0, -16(%rsp)
	movq	$0, -8(%rsp)
	movabsq	$4614388178203810202, %rax # imm = 0x400999999999999A
	movq	%rax, -24(%rsp)
	movsd	.LCPI0_0(%rip), %xmm0   # xmm0 = mem[0],zero
	movsd	.LCPI0_1(%rip), %xmm1   # xmm1 = mem[0],zero
	jmp	.LBB0_1
	.p2align	4, 0x90
.LBB0_7:                                # %then
                                        #   in Loop: Header=BB0_1 Depth=1
	addsd	%xmm1, %xmm2
	movsd	%xmm2, -24(%rsp)
.LBB0_1:                                # %loop1
                                        # =>This Inner Loop Header: Depth=1
	movl	-28(%rsp), %eax
	cmpl	$8, %eax
	xorps	%xmm2, %xmm2
	movapd	%xmm0, %xmm3
	jb	.LBB0_3
# %bb.2:                                # %loop1
                                        #   in Loop: Header=BB0_1 Depth=1
	xorpd	%xmm3, %xmm3
.LBB0_3:                                # %loop1
                                        #   in Loop: Header=BB0_1 Depth=1
	ucomisd	%xmm2, %xmm3
	je	.LBB0_9
# %bb.4:                                # %loop2
                                        #   in Loop: Header=BB0_1 Depth=1
	movsd	-24(%rsp), %xmm2        # xmm2 = mem[0],zero
	addsd	%xmm0, %xmm2
	movsd	%xmm2, -24(%rsp)
	incl	%eax
	movl	%eax, -28(%rsp)
	cmpl	$4, %eax
	xorpd	%xmm3, %xmm3
	movapd	%xmm0, %xmm4
	je	.LBB0_6
# %bb.5:                                # %loop2
                                        #   in Loop: Header=BB0_1 Depth=1
	xorpd	%xmm4, %xmm4
.LBB0_6:                                # %loop2
                                        #   in Loop: Header=BB0_1 Depth=1
	ucomisd	%xmm3, %xmm4
	jne	.LBB0_7
# %bb.8:                                # %else
                                        #   in Loop: Header=BB0_1 Depth=1
	addsd	%xmm0, %xmm2
	movsd	%xmm2, -24(%rsp)
	jmp	.LBB0_1
.LBB0_9:                                # %afterloop
	xorpd	%xmm0, %xmm0
	retq
.Lfunc_end0:
	.size	func, .Lfunc_end0-func
	.cfi_endproc
                                        # -- End function

	.section	".note.GNU-stack","",@progbits
