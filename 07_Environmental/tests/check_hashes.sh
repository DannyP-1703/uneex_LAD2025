#!/usr/bin/env bash
set -euo pipefail

PROG="$1"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TESTFILE="${SCRIPT_DIR}/testfile.txt"

if [ ! -x "${PROG}" ]; then
  echo "Executable '${PROG}' not found or not executable" >&2
  exit 2
fi

get_rhash() {
  local algo="$1"
  local msg="$2"
  printf "%s %s\n" "${algo^^}" "${msg}" | "${PROG}" | grep -Eo '[0-9a-fA-F]{32,40}' | head -n1
}

sha1_rhash=$(get_rhash sha1 "${TESTFILE}")
sha1_ref=$(sha1sum -t "${TESTFILE}" | cut -d' ' -f1)
if [ "${sha1_rhash}" != "${sha1_ref}" ]; then
  echo "SHA1 mismatch: rhasher='${sha1_rhash}' ref='${sha1_ref}'" >&2
  exit 1
fi

md5_rhash=$(get_rhash md5 "${TESTFILE}")
md5_ref=$(md5sum -t "${TESTFILE}" | cut -d' ' -f1)
if [ "${md5_rhash}" != "${md5_ref}" ]; then
  echo "MD5 mismatch: rhasher='${md5_rhash}' ref='${md5_ref}'" >&2
  exit 1
fi

echo "OK"
exit 0
