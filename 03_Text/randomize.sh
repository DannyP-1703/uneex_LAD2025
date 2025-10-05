#!/bin/bash

delay=${1:-0.2}

i=0
j=0
chars=( )
while IFS= read -r -n1 char ; do
    case $char in
        $'\n')
            i=$((i+1))
            j=0
            ;;
        *)
            chars+=("$i $j $(printf "%s" "$char" | od -v -An -tx1)"$'\n')
            j=$((j+1))
            ;;
    esac    
done


echo ${chars[@]} #| shuf | while read -r x y c ; do
