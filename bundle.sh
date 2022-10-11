#!/bin/bash

# single-file library bundler
# https://github.com/nothings/stb
# https://github.com/nothings/stb/blob/master/docs/stb_howto.txt

set -o nounset -o pipefail -o errexit

URL=${URL-https://github.com/rootmos/libr}
OUTPUT=/dev/stdout
while getopts "o:-" OPT; do
    case $OPT in
        o) OUTPUT=$OPTARG ;;
        -) break ;;
        ?) exit 2 ;;
    esac
done
shift $((OPTIND-1))

SCRIPT_DIR=$(readlink -f "$0" | xargs dirname)
BASE_DIR=${BASE_DIR-$SCRIPT_DIR}
VERSION=${VERSION-$(git -C "$BASE_DIR" rev-parse HEAD)}
SRC=${SRC-$BASE_DIR/src}
INC=${INC-$BASE_DIR/include/r}

TMP=$(mktemp -d)
trap 'rm -rf $TMP' EXIT

readarray -t ALL_MODULES < <(find "$SRC" -name "*.c" -exec basename --suffix=.c {} \;)

if [ $# -gt 0 ]; then
    for i in "$@"; do
        echo "$i" >> "$TMP/choices"
    done
else
    DIALOG_ARGS=()
    for m in "${ALL_MODULES[@]}"; do
        DIALOG_ARGS+=("$m" "$m" "off")
    done
    dialog --separate-output --no-tags --checklist "select modules" 0 0 0 \
        "${DIALOG_ARGS[@]}" 2>"$TMP/choices"
fi

is_selected() {
    grep -cFxq "$@" "$TMP/choices"
}

depends() {
    if is_selected "$1" && ! is_selected "$2"; then
        echo "$2" >> "$TMP/choices"
    fi
}

depends "stopwatch" "fail"
depends "stopwatch" "logging"
depends "xorshift" "fail"
depends "mark" "fail"
depends "mark" "logging"
depends "test" "util"
depends "test" "fail"
depends "file" "fail"
depends "file" "logging"
depends "logging" "fail"
depends "fail" "logging"
depends "logging" "util"
depends "landlock" "fail"
depends "lua" "fail"

readarray -t MODULES < "$TMP/choices"

echo 1>&2 "included modules: ${MODULES[*]}"

echo >"$OUTPUT" "// libr ($VERSION) ($URL) ($(date -Is))"
echo >>"$OUTPUT" "// modules: ${MODULES[*]}"
echo >>"$OUTPUT" ""

echo >>"$OUTPUT" "#ifndef LIBR_HEADER"
echo >>"$OUTPUT" "#define LIBR_HEADER"
for m in "${MODULES[@]}"; do
    echo >>"$OUTPUT" ""
    echo >>"$OUTPUT" "// libr: $m.h"
    tail -n+3 "$INC/$m.h" >>"$OUTPUT" # HACK: remove pragma once directive
done
echo >>"$OUTPUT" ""
echo >>"$OUTPUT" "#endif // LIBR_HEADER"

echo >>"$OUTPUT" "#ifdef LIBR_IMPLEMENTATION"
for m in "${MODULES[@]}"; do
    echo >>"$OUTPUT" ""
    echo >>"$OUTPUT" "// libr: $m.c"
    grep -vx '#include "r/\w\+.h"' "$SRC/$m.c" >>"$OUTPUT"
done
echo >>"$OUTPUT" ""
echo >>"$OUTPUT" "#endif // LIBR_IMPLEMENTATION"
