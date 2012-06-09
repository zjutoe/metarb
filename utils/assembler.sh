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

# segment 32 continuous chars to 4x8
inst_seg() {
    inst=$1
    echo "$(cut -c 1-4 <<< $inst) $(cut -c 5-8 <<< $inst) $(cut -c 9-12 <<< $inst) $(cut -c 13-16 <<< $inst)\
         $(cut -c 17-20 <<< $inst) $(cut -c 21-24 <<< $inst) $(cut -c 25-28 <<< $inst) $(cut -c 29-32 <<< $inst)"
}

# assume the input is segmented as 4x8
inst_hex() {
    read -r s0 s1 s2 s3 s4 s5 s6 s7 <<< $1
    out=""
    for s in  $s0 $s1 $s2 $s3 $s4 $s5 $s6 $s7; do
	h="$(./lua52 base_bin2hex.lua $s)"
	out="$out$h"
    done
    
    echo "$out"
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

	bin=$(bc <<< "obase=2;$dec")

	# pad bin to 16 bits
	binlen=${#bin}
	if [ $binlen -lt 16 ]; then
	    pad="0000000000000000"
	    pad=${pad:0:$((16 - $binlen))}
	    bin=$pad$bin
	fi

	echo $bin
}

output_inst() {
    inst_bin_full=$1
    inst_bin_seg="$(cut -c 1-4   <<< $inst_bin_full) $(cut -c 5-8   <<< $inst_bin_full)\
                  $(cut -c 9-12  <<< $inst_bin_full) $(cut -c 13-16 <<< $inst_bin_full)\
                  $(cut -c 17-20 <<< $inst_bin_full) $(cut -c 21-24 <<< $inst_bin_full)\
                  $(cut -c 25-28 <<< $inst_bin_full) $(cut -c 29-32 <<< $inst_bin_full)"
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

    output_inst "$b_op$b_rs$b_rt$b_sa$b_rd$b_func"
    
    # inst="$b_op""$b_rs""$b_rt""$b_sa""$b_rd""$b_func"
    # segs=$(inst_seg $inst)
    # # inst_seg="$(cut -c 1-4 <<< $inst) $(cut -c 5-8 <<< $inst) $(cut -c 9-12 <<< $inst) $(cut -c 13-16 <<< $inst)\
    # #      $(cut -c 17-20 <<< $inst) $(cut -c 21-24 <<< $inst) $(cut -c 25-28 <<< $inst) $(cut -c 29-32 <<< $inst)"

    # echo $inst -- $segs -- 0x$(inst_hex "$segs")
}

do_i_type_a() {
    b_op=$(opcode $op)
    b_rs=$(regno $rs)
    b_rt=$(regno $rt)
    b_imm=$(dec2bin $imm)

    output_inst "$b_op$b_rs$b_rt$b_imm"

    # inst="$b_op$b_rs$b_rt$b_imm"
    # segs=$(inst_seg $inst)
    # echo $inst -- $segs -- 0x$(inst_hex "$segs")
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
		do_i_type_b1	# I-type branch 1
		;;

	    'bgez' | 'bgtz'| 'blez' | 'bltz')
 		read -r _ rs label _ <<< $fields
		do_i_type_b2	# I-type branch 2
		;;

	    'lb' | 'lbu' | 'lh' | 'lhu' | 'lw'  | 'lwcl' |\
            'sb' | 'sh' | 'sw' | 'swcl')
		read -r _ rt addr <<< $fields
		
		# to split the addr as 'imm(rs)' into 'imm rs'
		#   maybe we should use sed instead
		addr=$(awk -F "[\(\)]" '{print $1 " " $2}' <<< $addr)

		read -r imm rs <<< $addr

		do_i_type_ls	# I-type load store
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
