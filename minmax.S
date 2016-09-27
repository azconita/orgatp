#include <mips/regdef.h>

	.text
	.align	2

	.globl max
	.globl min

	.ent max
max:
	.frame	$fp, 16, ra
	.set	noreorder
	.cpload	t9
	.set	reorder
	subu	sp, sp, 16
	.cprestore 0
	sw		gp, 4(sp)
	sw		$fp, 8(sp)
	move	$fp, sp
	sw		a0, 16(sp)
	sw 		a1, 20(sp)

	bgt		a0, a1, maxa0
	move 	v0, a1
	b       fin
maxa0:
	move	v0, a0
fin:
	#se devuelve en v0 a o b
	lw		gp, 4(sp)
	lw		$fp, 8(sp)
	j 		ra
	.end max

#function min(a,b)
	.ent min
min:
	.frame	$fp, 16, ra
	.set	noreorder
	.cpload	t9
	.set	reorder
	subu	sp, sp, 16
	.cprestore 0
	sw		gp, 4(sp)
	sw		$fp, 8(sp)
	move	$fp, sp
	sw		a0, 16(sp)
	sw 		a1, 20(sp)

	#inverso de max
	blt		a0, a1, mina0
	move 	v0, a1
	b       nim
mina0:
	move	v0, a0
nim:
	#se devuelve en v0 a o b
	lw		gp, 4(sp)
	lw		$fp, 8(sp)
	j 		ra

	.end min 