func_code = {
   ['add']   =  '100000',
   ['addu']  =  '100001',
   ['and']   =  '100100',
   ['break'] =  '001101',

   ['div']   = '011010',
   ['divu']  = '011011',
   ['jalr']  = '001001', 
   ['jr']    = '001000',

   ['mfhi']  = '010000',
   ['mflo']  = '010010',
   ['mthi']  = '010001',
   ['mtlo']  = '010011',

   ['mult']  = '011000',
   ['multu'] = '011001',
   ['nor']   = '100111',
   ['or']    = '100101',

   ['sll']   = '000000',
   ['sllv']  = '000100',
   ['slt']   = '101010',
   ['sltu']  = '101011',

   ['sra']   = '000011',
   ['srav']  = '000111',
   ['srl']   = '000010',
   ['srlv']  = '000110',

   ['sub']     = '100010',
   ['subu']    = '100011',
   ['syscall'] = '001100',	-- OK, so syscall is R-type...
   ['xor']     = '100110',
}

print(func_code[arg[1]])
