#!/bin/sh

regno() {
    ./lua52 regno.lua $1
}

r_type_func() {
    ./lua52 r_type_func.lua $1
}

inst_hex() {
    read -r s0 s1 s2 s3 s4 s5 s6 s7 <<< $1
    for s in  $s0 $s1 $s2 $s3 $s4 $s5 $s6 $s7; do
	h="$(./lua52 base_bin2hex.lua $s)"
	inst_hex="$inst_hex$h"
    done
    
    echo "$inst_hex"
}

do_r_type() {
    
    #echo "R-type" $op $rd  $rs $rt

    b_op='000000'
    b_rs=$(regno $rs)
    b_rt=$(regno $rt)
    b_rd=$(regno $rd)
    b_sa='00000'
    b_func=$(r_type_func $op)
    
    inst="$b_op""$b_rs""$b_rt""$b_sa""$b_rd""$b_func"
    inst_seg="$(cut -c 1-4 <<< $inst) $(cut -c 5-8 <<< $inst) $(cut -c 9-12 <<< $inst) $(cut -c 13-16 <<< $inst)\
         $(cut -c 17-20 <<< $inst) $(cut -c 21-24 <<< $inst) $(cut -c 25-28 <<< $inst) $(cut -c 29-32 <<< $inst)"

    echo $inst -- $inst_seg -- $(inst_hex "$inst_seg")
}


main() {

	fields=$(echo $inst | sed 's/\,//g')
	read -r op rd rs rt _ <<< $fields

	case $op in
	    'add' | 'addu' | 'and' | 'div'   | 'divu' |\
	    'mfhi'| 'mflo' | 'mult'| 'multu' | 'or'   |\
	    'slt' | 'sltu' | 'sll' | 'sllv'  | 'sra'  | 'srl' | 'srlv' |\
	    'sub' | 'subu' | 'xor')
		do_r_type
		;;
	    
	    *)
		echo "invalid"
		;;
	esac

}


inst=$@

main
