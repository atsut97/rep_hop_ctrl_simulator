#!/bin/sh

if [ -z $1 ]; then
  echo "please specify test program to run"
  echo "for example,"
  echo "  ./run_test.sh slip_test"
  exit 1
fi

TEST="$1"
shift
ARGS="$@"
make
./$TEST "$ARGS" > $TEST.csv
if [ -f Pipfile ]; then
  pipenv run ./plot_one_hop.py $TEST.csv
fi
