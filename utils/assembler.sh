#!/bin/sh

regno() {
    ./lua52 regno.lua $1
}

r_type_func() {
    ./lua52 r_type_func.lua $1
}

do_r_type() {
    
    #echo "R-type" $op $rd  $rs $rt

    b_op='000000'
    b_rs=$(regno $rs)
    b_rt=$(regno $rt)
    b_rd=$(regno $rd)
    b_sa='00000'
    b_func=$(r_type_func $op)
    
    echo "$b_op""$b_rs""$b_rt""$b_sa""$b_rd""$b_func"

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
