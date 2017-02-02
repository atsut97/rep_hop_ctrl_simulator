#!/bin/sh

# usage
usage () {
    echo "./test_runner <options> test1 test2 ..."
    echo "Arguments:"
    echo "\tSpecify tests to be executed. If none of tests"
    echo "\tis specified all the tests will be executed."
    echo ""
    echo "Options:"
    echo "\t-h|-help      show this message and exit."
    echo "\t-m|-memcheck  check memory leak using valgrind."
}

# parse arguments
MEMCHECK=false
TESTS=
while [ $# -gt 0 ]; do
    case "$1" in
        -help | -h)     usage
                        exit 0
                        ;;
        -memcheck | -m) MEMCHECK=true
                        shift
                        ;;
        -*)             usage
                        exit 1
                        ;;
        *)              TESTS="$TESTS ./$1"
                        shift
                        ;;
    esac
done

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

# check if a command is available
command_exist () {
    command -v $1 >/dev/null 2>&1
    ret=$?
    if [ $ret -gt 0 ]; then
        echo "$1 is not available"
        echo "try 'sudo apt-get install $1'"
    fi
    return $ret
}

# echo running test name and run the test
# argumets: a test to run
# output: return value of the test
run_test () {
    test_name=`echo $i | sed -e "s/^..//g"`
    echo "\nRunning $test_name..."
    $VALGRIND$1
    return $?
}

# report all of the arguments as failed
report_failed_tests () {
    printf "\n"
    printf "\033[31m==============================\n"
    printf "\033[31m There are some failed tests!\n"
    for i in $@; do
        printf "\033[31m   * $i\n"
    done
    printf "\033[31m==============================\n"
}

# show tests are passed
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

#
# main part starts from here
#

# if no test is specified find all tests
if [ -z $TESTS ]; then
    TESTS=`find_exe .`
fi
# check memcheck flag
VALGRIND=
if [ $MEMCHECK = true ]; then
    command_exist valgrind
    ret=$?
    if [ $ret -eq 0 ]; then
        VALGRIND="valgrind -q --leak-check=full --error-exitcode=1 "
    fi
fi

SKIPPED_TESTS="./rhc_test_test"
FAILED_TESTS=""
for i in $TESTS; do
    contain $i $SKIPPED_TESTS
    ret=$?
    if [ $ret -eq 0 ]; then
        # echo "$i is skipped"
        continue
    fi
    run_test $i
    if [ $? -gt 0 ]; then
        FAILED_TESTS="$FAILED_TESTS $test_name"
    fi
done

report $FAILED_TESTS
