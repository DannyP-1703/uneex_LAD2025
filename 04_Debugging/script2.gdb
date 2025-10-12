set pagination off
set $cnt = 0
b 54 if ++$cnt >= 28 && $cnt <= 35
command 1
        printf "@@@ %d %d %d %d\n", start, end, step, i
        cont
end

run -100 100 3 >/dev/null
quit