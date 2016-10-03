#include <mips/regdef.h>

	.text
	.align	2

	.globl vecinos
	.ent vecinos
vecinos: 
	.frame	$fp, 64, ra
	.set	noreorder
	.cpload	t9
	.set	reorder
	subu	sp, sp, 64
	.cprestore 0

	sw		$fp,56(sp)
	sw		gp,52(sp)
	sw		ra,48(sp)
	move	$fp,sp

#guardo variables
	sw		a0, 64(sp)
	sw		a1, 68(sp)
	sw		a2, 72(sp)
	sw		a3, 76(sp)


#computar maximos y minimos
	#max i -> desde: i_origen
	move 	a0, zero 
	beq		a1, zero, maxi0
	subu 	a1, a1, 1 #otra funcion?
	#jal 	max
	#sw		v0, 28(sp)
	bgt		a0, a1, maxi0
	sw		a1, 28(sp)
	b       salmaxi
maxi0:
	sw		a0, 28(sp)
salmaxi:

	#min i-> hasta: i_final
	lw		a0, 76(sp)
	subu 	a0, a0, 1
	lw		a1, 68(sp)
	addu 	a1, a1, 1
	#jal		min
	#sw		v0, 32(sp)
	blt		a0, a1, mini0
	sw		a1, 32(sp)
	b       salmini
mini0:
	sw		a0, 32(sp)
salmini:

	#max j-> desde: j_origen
	move	a0, zero
	lw		a1, 72(sp)
	beq		zero, a1, maxj0
	subu 	a1, a1, 1
	#jal		max
	#sw		v0, 36(sp)
	bgt		a0, a1, maxj0
	sw 	a1, 36(sp)
	b       salmaxj
maxj0:
	sw	a0, 36(sp)
salmaxj:

	#min j-> hasta: j_final
	lw		a0, 80(sp)
	subu 	a0, a0, 1
	lw		a1, 72(sp)
	addu 	a1, a1, 1
	#jal		min
	#sw		v0, 40(sp)
	blt		a0, a1, minj0
	sw		a1, 40(sp)
	b       salminj
minj0:
	sw 		a0, 40(sp)
salminj:

#computar vecinos
#usar los s0-s7???
	move	t0, zero	#total
	lw		t1, 28(sp)#i = i_origen
	lw		t2, 32(sp)#i_final
	lw		t3, 36(sp)#j
	lw		t4, 36(sp)#j_origen
	lw		t5, 40(sp)#j_final
	lw		a0, 64(sp)#matrix 
	lw		a1, 68(sp) #row
	lw		a2, 72(sp)#column
	lw		a3, 80(sp)#columns


loop1:
	bgt 	t1, t2, pool1
	move	t3, t4
loop2:
	bgt		t3, t5, pool2
	bne		t1, a1, sum
	bne		t3, a2, sum
	b 		mus
sum: #esto no va a funcionar
	mul		t8, t1, a3 #columns * row
	#mflo	t8
	addu 	t8, t8, t3 # + column
	li 		t7, 4
	mul 	t8, t8, t7 # * 4 por el tamaño de palabra en memoria
	#mflo	t8
	addu 	t8, a0, t8 #dirección que queremos leer
	lw 		t9, 0(t8) #esto menos -> cargamos lo que queremos leer
	addu 	t0, t0, t9 #lo sumamos al total parcial

mus:
	addu	t3, t3, 1
	b 		loop2

pool2:	
	addu	t1, t1, 1
	b 		loop1
pool1:
	move	v0, t0


	lw		$fp, 56(sp)
	lw		gp, 52(sp)
	lw		ra, 48(sp)
	addu 	sp, sp, 64
	j 		ra
	.end	vecinos
