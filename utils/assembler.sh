#!/bin/sh

do_r_type() {
    
    echo "R-type" $op $rd  $rs $rt

    b_op='000000'
    b_rs=regno $rs
    b_rt=regno $rt
    b_rd=regno $rd
    b_sa='00000'
    
    

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
