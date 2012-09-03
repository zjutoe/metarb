#!/bin/bash

grep '^#define[[:blank:]]*R_' inst.h | while read -r a func _; do
    op_name=$(cut -f2 -d_ <<< $func)
    echo "case $func:"; 
    echo "    exec_${op_name}(core, inst);"
    echo "    break;"
done
