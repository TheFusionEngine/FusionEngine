	.file	1 "vfpu_atanf.c"
	.section .mdebug.eabi32
	.section .gcc_compiled_long32
	.previous
	.text
	.align	2
	.globl	vfpu_atanf
	.ent	vfpu_atanf
vfpu_atanf:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	mfc1	$2,$f12
	#nop
 #APP
	mtv      $2, S000
vmul.s   S001, S000, S000
vadd.s   S001, S001, S001[1]
vrsq.s   S001, S001
vmul.s   S000, S000, S001
vasin.s  S000, S000
vcst.s   S001, VFPU_PI_2
vmul.s   S000, S000, S001
mfv      $2, S000

 #NO_APP
	mtc1	$2,$f0
	j	$31
	.end	vfpu_atanf
	.size	vfpu_atanf, .-vfpu_atanf
	.section	.rodata.cst4,"aM",@progbits,4
	.align	2
$LC0:
	.word	1078530011
	.align	2
$LC1:
	.word	-1068953637
	.align	2
$LC2:
	.word	-1077342245
	.align	2
$LC3:
	.word	1070141403
	.text
	.align	2
	.globl	vfpu_atan2f
	.ent	vfpu_atan2f
vfpu_atan2f:
	.frame	$sp,16,$31		# vars= 0, regs= 1/2, args= 0, gp= 0
	.mask	0x80000000,-16
	.fmask	0x00300000,-4
	mtc1	$0,$f0
	addiu	$sp,$sp,-16
	c.eq.s	$f12,$f0
	swc1	$f21,12($sp)
	swc1	$f20,8($sp)
	sw	$31,0($sp)
	mov.s	$f20,$f12
	.set	noreorder
	.set	nomacro
	bc1f	$L4
	mov.s	$f21,$f13
	.set	macro
	.set	reorder

	c.eq.s	$f13,$f0
	#nop
	.set	noreorder
	.set	nomacro
	bc1t	$L27
	lw	$31,0($sp)
	.set	macro
	.set	reorder

$L4:
 #APP
	abs.s $f1, $f21
	abs.s $f0, $f20
 #NO_APP
	c.le.s	$f0,$f1
	#nop
	bc1t	$L25
	.set	noreorder
	.set	nomacro
	jal	vfpu_atanf
	div.s	$f12,$f21,$f20
	.set	macro
	.set	reorder

	mov.s	$f1,$f0
	mtc1	$0,$f0
	#nop
	c.lt.s	$f20,$f0
	#nop
	.set	noreorder
	.set	nomacro
	bc1t	$L26
	lui	$2,%hi($LC3)
	.set	macro
	.set	reorder

	lwc1	$f0,%lo($LC3)($2)
	add.s	$f0,$f0,$f1
$L9:
	lw	$31,0($sp)
$L27:
	lwc1	$f21,12($sp)
	lwc1	$f20,8($sp)
	.set	noreorder
	.set	nomacro
	j	$31
	addiu	$sp,$sp,16
	.set	macro
	.set	reorder

$L25:
	.set	noreorder
	.set	nomacro
	jal	vfpu_atanf
	div.s	$f12,$f20,$f21
	.set	macro
	.set	reorder

	mtc1	$0,$f1
	#nop
	c.lt.s	$f21,$f1
	#nop
	.set	noreorder
	.set	nomacro
	bc1f	$L27
	lw	$31,0($sp)
	.set	macro
	.set	reorder

	c.le.s	$f1,$f20
	#nop
	.set	noreorder
	.set	nomacro
	bc1f	$L15
	lui	$2,%hi($LC0)
	.set	macro
	.set	reorder

	lwc1	$f1,%lo($LC0)($2)
	.set	noreorder
	.set	nomacro
	j	$L27
	add.s	$f0,$f0,$f1
	.set	macro
	.set	reorder

$L26:
	lui	$2,%hi($LC2)
	lwc1	$f0,%lo($LC2)($2)
	.set	noreorder
	.set	nomacro
	j	$L9
	add.s	$f0,$f0,$f1
	.set	macro
	.set	reorder

$L15:
	lui	$2,%hi($LC1)
	lwc1	$f1,%lo($LC1)($2)
	.set	noreorder
	.set	nomacro
	j	$L9
	add.s	$f0,$f0,$f1
	.set	macro
	.set	reorder

	.end	vfpu_atan2f
	.size	vfpu_atan2f, .-vfpu_atan2f
	.ident	"GCC: (GNU) 4.0.2 (PSPDEV 20051022)"
