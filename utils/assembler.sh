#!/bin/sh

regno() {
    ./lua52 regno.lua $1
}

r_type_func() {
    ./lua52 r_type_func.lua $1
}

opcode() {
    ./lua52 opcode.lua $1
}


bin2hex() {
    echo "ibase=2;obase=F;$1" | bc
}

dec2bin() {
	dec=$1
	
	# FIXME don't know how ot handle negetive numbers well
	# yet. I'll need to store them as 2's complement
	if [ $dec -lt 0 ]; then
	    dec=$((-$dec))
	    neg=1
	else
	    neg=0
	fi

	echo "obase=2;$dec" | bc
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

do_i_type_a() {
    b_op=$(opcode $op)
    b_rs=$(regno $rs)
    b_rt=$(regno $rt)
    b_imm=$(dec2bin $imm)
    b_imm=$(pad_zero_pre $b_imm 16)

    output_inst "$b_op$b_rs$b_rt$b_imm"
}


do_i_type_b() {
    b_op=$(opcode $op)
    b_rs=$(regno $rs)
    b_rt=$(regno $rt)
    b_label=$(dec2bin $label)
    b_label=$(pad_zero_pre $b_label 16)

    output_inst "$b_op$b_rs$b_rt$b_label"
}


do_i_type_ls() {
    echo 'I-type L/S' $op $rt $imm $rs
}



main() {

	fields=$(echo $inst | sed 's/\,//g')
	read -r op _ <<< $fields

	case $op in
	    'add' | 'addu' | 'and' | 'div'   | 'divu' |\
	    'mfhi'| 'mflo' | 'mult'| 'multu' | 'or'   |\
	    'slt' | 'sltu' | 'sll' | 'sllv'  | 'sra'  | 'srl' | 'srlv' |\
	    'sub' | 'subu' | 'xor')
		read -r _ rd rs rt _ <<< $fields
		do_r_type
		;;

	    'addi' | 'addiu' | 'andi' | 'ori' |\
            'slti' | 'sltiu'  | 'xori')
		read -r _ rt rs imm _ <<< $fields
		do_i_type_a	# I-type arithmetic
		;;

	    'beq' | 'bne')
		read -r _ rs rt label _ <<< $fields
		do_i_type_b	# I-type branch 1
		;;

	    'bgez' | 'bgtz'| 'blez' | 'bltz')
 		read -r _ rs label _ <<< $fields
		if [ $op = 'bgez' ]; then
		    rt='$1'
		else
		    rt='$0'
		fi
		do_i_type_b	# I-type branch 2
		;;

	    'lb' | 'lbu' | 'lh' | 'lhu' | 'lw'  | 'lwcl' |\
            'sb' | 'sh' | 'sw' | 'swcl')
		read -r _ rt addr <<< $fields
		
		# to split the addr as 'imm(rs)' into 'imm rs'
		#   maybe we should use sed instead
		addr=$(awk -F "[\(\)]" '{print $1 " " $2}' <<< $addr)

		read -r imm rs <<< $addr

		do_i_type_a	# I-type load store
		;;
	    'lui')
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
