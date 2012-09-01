#!/bin/bash

grep '#define OP_' inst.h | while read -r a op _; do
    op_name=$(cut -f2 -d_ <<< $op)
    echo "case $op:"; 
    echo "    exec_${op_name}(core, inst);"
    echo "    break;"
done
