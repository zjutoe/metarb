#!/bin/bash

regno() {
    lua regno.lua $1
}

r_type_func() {
    lua r_type_func.lua $1
}

opcode() {
    lua opcode.lua $1
}


bin2hex() {
    echo "obase=16;ibase=2;$1" | bc
}

# arguments:
#  $1: the src dec number
#  $2: the data width
dec2bin() {
	dec=$1
	t_width=$2
	
	if [ $dec -lt 0 ]; then
	    # 2's complement representation of negative number
	    echo "obase=2;(2^$t_width)+($dec)" | bc
	else
	    echo "obase=2;$dec" | bc
	fi
}


# assume the input is segmented as 4x8
inst_hex() {
    read -r s0 s1 s2 s3 s4 s5 s6 s7 <<< $1
    out=""
    for s in  $s0 $s1 $s2 $s3 $s4 $s5 $s6 $s7; do
	h=$(bin2hex $s)
	out="$out$h"
    done
    
    echo "$out"
}

# aguments
#  $1: the string to be padded
#  $2: the length of the padded result, MAX=32
pad_zero_pre() {
    t_src=$1
    t_srclen=${#t_src}

    # MAX=32
    t_pad="00000000000000000000000000000000"
    t_pad=${t_pad:0:$(($2 - $t_srclen))}

    echo "$t_pad$t_src"
}



output_inst() {
    inst_bin_full=$1
    inst_bin_seg="${inst_bin_full:0:4} ${inst_bin_full:4:4} ${inst_bin_full:8:4} ${inst_bin_full:12:4}\
                  ${inst_bin_full:16:4} ${inst_bin_full:20:4} ${inst_bin_full:24:4} ${inst_bin_full:28:4}"
    inst_hex_full=0x$(inst_hex "$inst_bin_seg")

    echo $inst_bin_full -- $inst_bin_seg -- $inst_hex_full -- $inst
}    

do_r_type() {
    
    #echo "R-type" $op $rd  $rs $rt

    b_op='000000'
    b_rs=$(regno $rs)
    b_rt=$(regno $rt)
    b_rd=$(regno $rd)
    b_sa='00000'
    b_func=$(r_type_func $op)

    output_inst "$b_op$b_rs$b_rt$b_rd$b_sa$b_func"
}

do_i_type() {
    b_op=$(opcode $op)
    b_rs=$(regno $rs)
    b_rt=$(regno $rt)
    b_imm=$(dec2bin $imm 16)
    b_imm=$(pad_zero_pre $b_imm 16) 
    # It's OK to pad negative numbers since them are already garanteed
    # to be 16 bits by dec2bin, i.e. no action will be taken for them
    # here.

    output_inst "$b_op$b_rs$b_rt$b_imm"
}

do_j_type() {
    b_op=$(opcode $op)
    b_label=$(dec2bin $inst_body 26)
    b_label=$(pad_zero_pre $b_label 26)
    # It's OK to pad negative numbers since them are already garanteed
    # to be 26 bits by dec2bin, i.e. no action will be taken for them
    # here.

    output_inst "$b_op$b_label"
}


main() {

	fields=$(echo $inst | sed 's/\,//g')
	read -r op inst_body <<< $fields

	case $op in
	    'add' | 'addu' | 'and'  | 'break' | 'jalr' | 'jr'    | 'div' | 'divu' |\
	    'mfhi'| 'mflo' | 'mthi' | 'mtlo'  | 'mult' | 'multu' | 'nor' | 'or'   |\
	    'sll' | 'sllv' | 'slt'  | 'sltu'  | 'sra'  | 'srav'  | 'srl' | 'srlv' |\
	    'sub' | 'subu' | 'syscall' | 'xor')
		rs='0'
		rt='0'
		rd='0'
		read -r _ rd rs rt _ <<< $fields
		do_r_type
		;;

	    # I-type arithmetic
	    'addi' | 'addiu' | 'andi' | 'ori' |\
            'slti' | 'sltiu' | 'xori')
		read -r _ rt rs imm _ <<< $fields
		do_i_type	
		;;

    	    # I-type branch 1
	    'beq' | 'bne')
		read -r _ rs rt imm _ <<< $fields
		do_i_type
		;;

	    'bgez' | 'bgtz'| 'blez' | 'bltz')
 		read -r _ rs imm _ <<< $fields
		if [ $op = 'bgez' ]; then
		    rt='$1'
		else
		    rt='$0'
		fi
		do_i_type	# I-type branch 2
		;;

	    'lb' | 'lbu' | 'lh' | 'lhu' | 'lw'  | 'lwcl' |\
            'sb' | 'sh' | 'sw' | 'swcl')
		read -r _ rt addr <<< $fields
		
		# to split the addr as 'imm(rs)' into 'imm rs'
		#   maybe we should use sed instead
		addr=$(awk -F "[\(\)]" '{print $1 " " $2}' <<< $addr)

		read -r imm rs <<< $addr

		do_i_type	# I-type load store
		;;
	    'lui')
		read -r _ rt imm <<< $fields
		rs='$0'
		do_i_type
		;;

	    'j' | 'jal')
		do_j_type
		;;
	    
	    *)
		echo "invalid instruction"
		;;
	esac

}

while read inst; do
    # echo $inst
    main
done
