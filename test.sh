#!/bin/bash
## # Brainfuck by FERcsI
#    ══════════════════════════════════════════════════════
#       Brainfuck interpreter
#       Copyright (c) 2023 Ferenc Vajda
#     ────────────────────────────────────────────────────
#       test.sh
#       Test script of brainfuck interpreter
#     ────────────────────────────────────────────────────
#       This code is licensed under the MIT license. See
#       the LICENSE file in the project root for license
#       terms.
#    ══════════════════════════════════════════════════════    */
#------------------------------------------------------------------File info-#[
#@ > - **File**: test.sh
#@ > - **Author**: VAJDA, Ferenc
#@ > - **Last modified**: 2023-03-02T16:35:43+0000
#]

declare -i failure_count=0

function failed {
    local fname outexp out errexp err
    fname="$1"
    outexp="$2"
    out="$3"
    errexp="$4"
    err="$5"
    echo 'FAILED'
    failure_count=$((failure_count + 1))
    echo "  $fname output"
    if [[ "$out" != $'\0' ]]; then
        echo "    stdout: $out" 1>&2
        echo "      exp.: $outexp" 1>&2
    fi
    if [[ "$err" != $'\0' ]]; then
        echo "    stderr: $err" 1>&2
        echo "      exp.: $errexp" 1>&2
    fi
}

function run_test {
    local text name outexp input opts errexp
    text="$1"
    name="$2"
    outexp="$3"
    input="$4"
    opts="$5"
    errexp="$6"
    local out err
    {
        read -r -d '' err
        read -r -d '' out
    } < <((printf '\0%s\0' "$(
        echo -n "$input" | ./brainfuck "test/$name.bf" $opts 
    )" 1>&2) 2>&1)

#>    echo -n "$name.bf: "
    echo -n "$text ($name.bf): "

    if [[ "$out" == "$outexp" ]]; then
        out=$'\0'
    fi
    if [[ "$err" == "$errexp" ]]; then
        err=$'\0'
    fi
    if [[ "$out" != $'\0' || "$err" != $'\0' ]]; then
        failed "test/$name.bf" "$outexp" "$out" "$errexp" "$err"
    else
        echo 'success'
    fi
}

# ======== RUN TESTS ========

version=$(./brainfuck --version)
if [[ "$version" == "" ]]; then
    echo "Cannot run tests, because interpreter is not available." 1>&2
    exit 1
fi
echo "Running tests for '$version'"
echo "----------------------------------------"

run_test 'Test output and basic instructions' \
    hello "Hello World!"

run_test 'Test stdin input' \
    reverse $'cba\n54321\nbye' $'abc\n12345\n'

run_test 'Test file input' \
    reverse $'cba\n54321\nbye' '' 'test/text'

run_test 'Test debug mode off' \
    debug $'ON\x08FF' $'\x1f'

debug_expected="["$(seq 31 -1 1 | xargs printf " %.2x")$'\e[1;31m 00\e[0m... ]\n[exit]'
run_test 'Test debug mode on' \
    debug $'ON' $'\x1f' "-d" "$debug_expected"

run_test 'Test comments with debug mode off' \
    comments "FAILURE"

run_test 'Test comments with debug mode on' \
    comments "SUCCESS" "" "-d"

debug_expected="["$(seq 3 -1 1 | xargs printf " %.2x")$'\e[1;31m 00\e[0m... ]\n[exit]'
run_test 'Test dump-size option' \
    debug $'ON' $'\x03' "-d -s 4" "$debug_expected"

run_test 'Test minify option with debug mode off' \
    small ",.>++++++[->++++++<]>-." '' --minify

run_test 'Test minify option with debug mode on' \
    small ",.>++++++[->++++++<]>.@$" '' '-d --minify'

echo "----------------------------------------"
echo "$failure_count tests failed"
if [[ "$failure_count" > 0 ]]; then
    exit 2
fi
exit 0
