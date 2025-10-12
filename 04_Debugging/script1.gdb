set pagination off
b 54 if i % 5 == 0
command 1
        printf "@@@ %d %d %d %d\n", start, end, step, i
        cont
end

run 1 12 >/dev/null
quit