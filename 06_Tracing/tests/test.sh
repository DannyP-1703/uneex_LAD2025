#!/usr/bin/env bash
set -euo pipefail

BIN="./move"
TESTDIR="tests"
INFILE="$TESTDIR/infile.tmp"
OUTFILE="$TESTDIR/outfile.tmp"
INFILE_PROT="$TESTDIR/infile_PROTECT.tmp"

trap 'clean' EXIT

clean() {
    rm -rf $INFILE $OUTFILE $INFILE_PROT
}

run_test() {
    local fault_spec="$1" expected_rc="$2" desc="$3" bin="$4" src="$5" dst="$6"

    clean

    echo "some data 42" > "$src"

    echo "Testing: $desc"
    if [ -n "${fault_spec}" ]; then
        strace -e fault=${fault_spec} "$bin" "$src" "$dst" > /dev/null 2>&1
        rc=$?
    else
        "$bin" "$src" "$dst" > /dev/null 2>&1
        rc=$?
    fi
    if [ $rc -ne $expected_rc ]; then
        echo "FAIL - expected rc $expected_rc, got $rc" >&2
        return 2
    fi
    if [ ! -f "$src" ]; then
        echo "FAIL  - source disappeared" >&2
        return 3
    fi
    if [ "$src" != "$dst" ]; then
        if [ -f "$dst" ]; then
            echo "FAIL - destination should not exist" >&2
            return 4
        fi
    fi
    echo "OK"
}

run_success() {
    local desc="$1" bin="$2" src="$3" dst="$4"

    clean

    echo "some data 42" > "$src"

    echo "Testing: $desc"

    "$bin" "$src" "$dst" > /dev/null 2>&1
    rc=$?

    if [ $rc -ne 0 ]; then
        echo "FAIL - expected rc 0, got $rc" >&2
        return 2
    fi
    if [ -f "$src" ]; then
        echo "FAIL  - source was not deleted" >&2
        return 3
    fi
    if [ ! -f "$dst" ]; then
        echo "FAIL - destination should exist" >&2
        return 4
    fi
    echo "OK"
}

run_usage_test() {
    local bin="$1"

    echo "Testing: usage"
    
    "$bin" > /dev/null 2>&1
    rc=$?

    if [ $rc -ne 1 ]; then
        echo "FAIL - expected rc 1, got $rc" >&2
        return 2
    fi
    echo "OK"
}

run_success "simple move" $BIN $INFILE $OUTFILE || true

run_success "move protected with no preload" $BIN $INFILE_PROT $OUTFILE || true

run_usage_test $BIN || true

run_test "newfstatat:error=ENOENT:when=3" 2 "stat(infile) failure" $BIN $INFILE $OUTFILE || true

run_test "openat:error=ENOENT:when=3" 4 "open(infile) failure" $BIN $INFILE $OUTFILE || true

run_test "openat:error=EACCES:when=4" 5 "open(outfile) failure" $BIN $INFILE $OUTFILE || true

run_test "read:error=EIO:when=2" 7 "read(infile) failure" $BIN $INFILE $OUTFILE || true

run_test "write:error=EIO:when=1" 8 "write(outfile) failure" $BIN $INFILE $OUTFILE || true

run_test "close:error=EIO:when=3" 9 "close(outfile) failure" $BIN $INFILE $OUTFILE || true

run_test "unlink:error=EPERM:when=1" 10 "unlink(infile) failure" $BIN $INFILE $OUTFILE || true

LD_PRELOAD=./libprotect.so run_test "" 10 "move protected with preload" $BIN $INFILE_PROT $OUTFILE || true

run_test "" 3 "same file" $BIN $INFILE $INFILE || true


clean
