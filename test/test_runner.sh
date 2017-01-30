#!/bin/sh

# find all executable files (excluding script files etc.)
# arguments: none
# output: found exes are output to stdout
find_exe () {
    found_exe=`find $1 -maxdepth 1 -type f -executable -exec file -i '{}' \; | grep 'x-executable; charset=binary' | sed -e "s/:.*$//g"`
    echo $found_exe
}

# check if a variable is contained in a list
# arguments: a variable to check and a list
# example: contain "var1" "var1 var2 var3"
# output: if the variable is contained in the list, return 0,
#         otherwise return 1
contain () {
    target=$1
    shift
    for e in "$@"; do
        if [ "$e" = "$target" ]; then
            return 0
        fi
    done
    return 1
}

# echo running test name and run the test
# argumets: a test to run
# output: return value of the test
run_test () {
    test_name=`echo $i | sed -e "s/^..//g"`
    echo "Running $test_name..."
    $1
    return $?
}

TESTS=`find_exe .`
SKIPPED_TESTS="./rhc_test_test"

FAILED_TESTS=""
for i in $TESTS; do
    ret=$(contain $i $SKIPPED_TESTS; echo $?)
    if [ $ret -eq 0 ]; then
        # echo "$i is skipped"
        continue
    fi
    run_test $i
    if [ $? -gt 0 ]; then
        FAILED_TESTS="$FAILED_TESTS $test_name"
    fi
done

report_failed_tests () {
    printf "\n"
    printf "\033[31m==============================\n"
    printf "\033[31m There are some failed tests!\n"
    for i in $@; do
        printf "\033[31m   * $i\n"
    done
    printf "\033[31m==============================\n"
}

report_all_green () {
    printf "\n"
    printf "\033[32mAll green!\n"
}

# report summary of the run tests
# arguments: a list of failed tests
report () {
    if [ $# -gt 0 ]; then
        report_failed_tests $@
    else
        report_all_green
    fi
}

report $FAILED_TESTS
