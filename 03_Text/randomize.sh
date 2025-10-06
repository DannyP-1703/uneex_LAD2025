#!/bin/bash

delay=${1:-0.05}

i=0
j=0
while IFS='' read -r -n1 char ; do
    if [ -z "$char" ] ; then
        i=$((i+1))
        j=0
    else
        if [ "$char" != ' ' ] ; then
            chars+=("$i $j$(printf "%s" "$char" | od -v -An -tx1)")
        fi
        j=$((j+1))
    fi
done

tput clear

{ for elem in "${chars[@]}"; do
    echo "$elem"
done } | shuf | { while read -r x y c ; do
    tput cup $x $y
    printf "\x$c"
    sleep $delay
done }

tput cup $i 0
