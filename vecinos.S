#include <mips/regdef.h>

	.text
	.align	2

	.extern min
	.extern max

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
	subu 	a1, a1, 1 #otra funcion?
	jal 	max
	sw		v0, 28(sp)

	#min i-> hasta: i_final
	lw		a0, 76(sp)
	subu 	a0, a0, 1
	lw		a1, 68(sp)
	addu 	a1, a1, 1
	jal		min
	sw		v0, 32(sp)

	#max j-> desde: j_origen
	move	a0, zero
	lw		a1, 72(sp)
	subu 	a1, a1, 1
	jal		max
	sw		v0, 36(sp)

	#min j-> hasta: j_final
	lw		a0, 80(sp)
	subu 	a0, a0, 1
	lw		a1, 72(sp)
	addu 	a1, a1, 1
	jal		min
	sw		v0, 40(sp)

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
	move	t4, t3
loop2:
	bgt		t3, t5, pool2
	bne		t1, a1, sum
	bne		t3, a2, sum
	b 		mus
sum: #esto no va a funcionar
	#mul		t8, a1, a3 #columns * row
	#addu 	t8, t8, a2 # + column
	#mul 	t8, t8, 4 # * 4 por el tamaño de palabra en memoria
	#addu 	t8, a0, t8 #esta suma no va a funcionar -> dirección que queremos leer
	#lw 		t9, (t8) #esto menos -> cargamos lo que queremos leer
	#addu 	t0, t0, t9 #lo sumamos al total parcial
	addu	t0, t0, 1

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




#function max(a,b)
	.ent maxim
maxim:
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
	addu	sp, sp, 16
	j 		ra
	.end maxim

#function min(a,b)
	.ent minim
minim:
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
	addu	sp, sp, 16
	j 		ra

	.end minim
