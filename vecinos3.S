#include <mips/regdef.h>

	.text
	.align	2

	.globl vecinos
	.ent vecinos
vecinos: 
	.frame	$fp, 32, ra
	.set	noreorder
	.cpload	t9
	.set	reorder
	subu	sp, sp, 32
	.cprestore 0

	sw		$fp,24(sp)
	sw		gp,20(sp)
	sw		ra,16(sp)
	move	$fp,sp

#guardo variables
	sw		a0, 32(sp)
	sw		a1, 36(sp)
	sw		a2, 40(sp)
	sw		a3, 44(sp)


#computar maximos y minimos
	#max i -> desde: i_origen
	move 	a0, zero 
	beq		a1, zero, maxi0
	subu 	a1, a1, 1
	bgt		a0, a1, maxi0
	sw		a1, 0(sp)
	b       salmaxi
maxi0:
	sw		a0, 0(sp)
salmaxi:

	#min i-> hasta: i_final
	lw		a0, 44(sp)
	subu 	a0, a0, 1
	lw		a1, 36(sp)
	addu 	a1, a1, 1
	blt		a0, a1, mini0
	sw		a1, 4(sp)
	b       salmini
mini0:
	sw		a0, 4(sp)
salmini:

	#max j-> desde: j_origen
	move	a0, zero
	lw		a1, 40(sp)
	beq		zero, a1, maxj0
	subu 	a1, a1, 1
	bgt		a0, a1, maxj0
	sw 	a1, 8(sp)
	b       salmaxj
maxj0:
	sw	a0, 8(sp)
salmaxj:

	#min j-> hasta: j_final
	lw		a0, 48(sp)
	subu 	a0, a0, 1
	lw		a1, 40(sp)
	addu 	a1, a1, 1
	blt		a0, a1, minj0
	sw		a1, 12(sp)
	b       salminj
minj0:
	sw 		a0, 12(sp)
salminj:

#computar vecinos
	move	t0, zero	#total
	lw		t1, 0(sp)#i = i_origen
	lw		t2, 4(sp)#i_final
	lw		t3, 8(sp)#j
	lw		t4, 8(sp)#j_origen
	lw		t5, 12(sp)#j_final
	lw		a0, 32(sp)#matrix 
	lw		a1, 36(sp)#row
	lw		a2, 40(sp)#column
	lw		a3, 48(sp)#columns


loop1:
	bgt 	t1, t2, pool1
	move	t3, t4
loop2:
	bgt		t3, t5, pool2
	bne		t1, a1, sum
	bne		t3, a2, sum
	b 		mus
sum:
	mul		t8, t1, a3 #columns * row
	addu 	t8, t8, t3 # + column
	li 		t7, 4
	mul 	t8, t8, t7 # * 4 por el tamano de palabra en memoria
	addu 	t8, a0, t8 #direccion que queremos leer
	lw 		t9, 0(t8) # cargamos lo que queremos leer
	addu 	t0, t0, t9 #lo sumamos al total parcial

mus:
	addu	t3, t3, 1
	b 		loop2

pool2:	
	addu	t1, t1, 1
	b 		loop1
pool1:
	move	v0, t0


	lw		$fp, 24(sp)
	lw		gp, 20(sp)
	lw		ra, 16(sp)
	addu 	sp, sp, 32
	j 		ra
	.end	vecinos
