regs = {}

regs['$zero'] = 0
regs['$at']   = 1
regs['$v0']   = 2
regs['$v1']   = 3
regs['$a0']   = 4
regs['$a1']   = 5
regs['$a2']   = 6
regs['$a3']   = 7
regs['$t0']   = 8
regs['$t1']   = 9
regs['$t2']   = 10
regs['$t3']   = 11
regs['$t4']   = 12
regs['$t5']   = 13
regs['$t6']   = 14
regs['$t7']   = 15
regs['$s0']   = 16
regs['$s1']   = 17
regs['$s2']   = 18
regs['$s3']   = 19
regs['$s4']   = 20
regs['$s5']   = 21
regs['$s6']   = 22
regs['$s7']   = 23
regs['$t8']   = 24
regs['$t9']   = 25
regs['$k0']   = 26
regs['$k1']   = 27
regs['$kt0']  = 26
regs['$kt1']  = 27
regs['$gp']   = 28
regs['$sp']   = 29
regs['$fp']   = 30
regs['$ra']   = 31

regs['$0']    = 0
regs['$1']    = 1
regs['$2']    = 2
regs['$3']    = 3
regs['$4']    = 4
regs['$5']    = 5
regs['$6']    = 6
regs['$7']    = 7
regs['$8']    = 8
regs['$9']    = 9
regs['$10']   = 10
regs['$11']   = 11
regs['$12']   = 12
regs['$13']   = 13
regs['$14']   = 14
regs['$15']   = 15
regs['$16']   = 16
regs['$17']   = 17
regs['$18']   = 18
regs['$19']   = 19
regs['$20']   = 20
regs['$21']   = 21
regs['$22']   = 22
regs['$23']   = 23
regs['$24']   = 24
regs['$25']   = 25
regs['$26']   = 26
regs['$27']   = 27
regs['$26']   = 26
regs['$27']   = 27
regs['$28']   = 28
regs['$29']   = 29
regs['$30']   = 30
regs['$31']   = 31

dec2bin = {
	[0] = '00000',
	[1] = '00001',
	[2] = '00010',
	[3] = '00011',
	[4] = '00100',
	[5] = '00101',
	[6] = '00110',
	[7] = '00111',

	[8]  = '01000',
	[9]  = '01001',
	[10] = '01010',
	[11] = '01011',
	[12] = '01100',
	[13] = '01101',
	[14] = '01110',
	[15] = '01111',

	[16] = '10000',
	[17] = '10001',
	[18] = '10010',
	[19] = '10011',
	[20] = '10100',
	[21] = '10101',
	[22] = '10110',
	[23] = '10111',

	[24] = '11000',
	[25] = '11001',
	[26] = '11010',
	[27] = '11011',
	[28] = '11100',
	[29] = '11101',
	[30] = '11110',
	[31] = '11111'
}

print(dec2bin[regs[arg[1]]])
