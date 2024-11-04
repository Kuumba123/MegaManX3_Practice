.set noreorder
jal SpawnBitCheck
nop
beq $v0, 0, 0x74
nop
j 0x80073fdc
nop
