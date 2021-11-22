#!/bin/sh

usage() {
  cat <<USAGE
Run example program and make plot.

Synopsis:
  $0 [-d <file>|--data=<file>] [-p <file>|--plot-script=<file>]
     [-D|--delete] [-P|--plot-only] [--no-make] [--no-plot]
     <file> [<arg>...]
  $0 [-h|--help]

Description:
  This script runs a specified example program and shows plotting of
  the produced data.

  To plot the data, a python script whose name matches the specified
  example program and ends with .py extension is used. Let's say you
  have an example program 'slip_test', you need a python script
  'slip_test.py' to plot the produced data. Or, you can specify an
  alternative script by using '--plot-script' option, such as
  'plot_one_hop.py'.

  By default, the data file produced from the program is named as the
  same as the program name with '.csv' extension, for example,
  'slip_test.csv'.

Options:
  <file>
    Program file to be executed.

  <arg>...
    Arguments to be passed to the example program.

  -d <file>, --data=<file>
    Use <file> as an intermediate data.

  -p <file>, --plot-script=<file>
    Specify a script file to execute to make plot.

  -D, --delete
    Delete the intermediate data file after plotting.

  -P, --plot-only
    Do not execute the program. Instead, plot using existing data
    file.

  --no-plot
    Do not plot.

  --no-make
    Do not run make.

  -n, --dry-run
    Just print commands to be executed if it were not for this option.

  -h, --help
    Show this message.

Examples:
  Execute 'slip_test', then show plot using 'slip_test.py'
    $0 slip_test

  Show plot using 'slip_test.py' with 'slip_test.csv'.
    $0 --plot-only slip_test

  Show plot using 'plot_one_hop.py' with 'slip_test.csv'.
    $0 --plot-script plot_one_hop.py slip_test

  Output data to 'slip_test.log' and use it to plot.
    $0 --data=slip_test.log slip_test

  Delete the data file after plotting.
    $0 --delete slip_test
USAGE
}

die() {
  if [ -p /dev/stdin ]; then
    cat >&2 </dev/stdin
  else
    echo >&2 "$@"
  fi
  exit 1
}

data=''
plot_script=''
delete=0
make=1
run=1
plot=1
dryrun=0
while :; do
  case $1 in
    -h|--help)
      usage
      exit
      ;;
    -d|--data)
      if [ -n "$2" ] && echo "$2" | grep -q -- '^[^-]'; then
        data="$2"
        shift
      else
        die "error: --data reauires a non-empty argument"
      fi
      ;;
    --data=?*)
      data="${1#*=}"
      ;;
    '--data=')
      die "error: --data reauires a non-empty argument"
      ;;
    -p|--plot-script)
      if [ -n "$2" ] && echo "$2" | grep -q -- '^[^-]'; then
        plot_script="$2"
        shift
      else
        die "error: --plot-script reauires a non-empty argument"
      fi
      ;;
    --plot-script=?*)
      plot_script="${1#*=}"
      ;;
    '--plot-script=')
      die "error: --plot-script reauires a non-empty argument"
      ;;
    -D|--delete)
      delete=1
      ;;
    -P|--plot-only)
      make=0
      run=0
      plot=1
      ;;
    --no-plot)
      plot=0
      ;;
    --no-make)
      make=0
      ;;
    -n|--dryrun|--dry-run)
      dryrun=1
      ;;
    --)
      shift
      break
      ;;
    -?*)
      echo >&2 "warning: unknown option: $1 (ignored)"
      ;;
    *)
      break
      ;;
  esac
  shift
done

# Cleans up the produced intermediate data file when exiting.
cleanup() {
  trap - EXIT
  if [ $((delete)) -ne 0 ]; then
    [ -n "$data" ] && rm -f "$data"
  fi
}
trap 'cleanup' EXIT

# Set shell options.
set -eu

# Exit with error when no program is specified.
if [ $# -eq 0 ]; then
  die <<EOF
error: program name is required
Example:
  $0 slip_test
EOF
fi

# Execute the provided arguments when --dry-option is not enable,
# otherwise just print them.
execcmd() {
  if [ $((dryrun)) -ne 0 ]; then
    echo "$@"
  else
    "$@"
  fi
}

# Run make.
if [ $((make)) -ne 0 ]; then
  execcmd make
fi

# Just concatenate a provided relative path with PWD variable. Note
# that it does not resolve symbolic links nor eliminate implied
# directory components such as '.' and '..'.
abspath() {
  _abspath=''
  case "$1" in
    /*) _abspath=$1 ;;
    *) _abspath=${PWD}/$1 ;;
  esac
  echo "$_abspath"
}

# Run program and redirect produced output to an intermediate data
# file.
program=$(abspath "$1"); shift
program_name=$(basename "$program")
if [ $((run)) -ne 0 ]; then
  ret=0
  if [ -f "$program" ]; then
    [ -z "$data" ] && data="${program_name}.csv"
    data=$(abspath "$data")
    if [ $((dryrun)) -ne 0 ]; then
      execcmd "$program" "$@" '>' "$data"
    else
      printf "%s" "Making plot data with $program_name ..."
      execcmd "$program" "$@" > "$data"
      ret=$?
      printf "%s\n" " Done."
    fi
  else
    die "error: $program not found"
  fi

  # When the program exits with error, abort it.
  if [ $((ret)) -ne 0 ]; then
    echo >&2 "error: ${program} exited with non-zero status"
    exit $ret
  fi
  unset ret
fi

# Terminate when --no-plot is enabled.
[ $((plot)) -eq 0 ] && exit 0

# Plot the data with an appropriate Python script.
scripts_directory=$(cd -- "$(dirname -- "$0")" && pwd -P)/scripts
if [ -n "$plot_script" ]; then
  # Fix path to the plot script specified by the user.
  if [ ! -f "$plot_script" ]; then
    if [ -f "${scripts_directory}/${plot_script}.py" ]; then
      plot_script=${scripts_directory}/${plot_script}.py
    elif [ -f "${scripts_directory}/${plot_script}" ]; then
      plot_script=${scripts_directory}/${plot_script}
    else
      die "error: specified plot script not found: $plot_script"
    fi
  fi
else
  # Find an accessible script since none is specified by the user.
  if [ -f "${scripts_directory}/${program_name}.py" ]; then
    plot_script=${scripts_directory}/${program_name}.py
  elif [ -f "${scripts_directory}/${program_name}" ]; then
    plot_script=${scripts_directory}/${program_name}
  else
    die <<EOF
error: Python script to plot not found
Or use plot_one_hop.py like as:
  $0 -p plot_one_hop slip_test
EOF
  fi
fi

cd "$scripts_directory" || exit 1
[ $((dryrun)) -ne 0 ] || printf "%s" "Running script to plot data ..."
if command -v poetry >/dev/null && \
    [ -f "${scripts_directory}/pyproject.toml" ]; then
  execcmd poetry run "$plot_script" "$data"
elif command -v pipenv >/dev/null && \
    [ -f "${scripts_directory}/Pipfile" ]; then
  execcmd pipenv run "$plot_script" "$data"
elif command -v python >/dev/null; then
  py_ver=$(python --version | cut -d' ' -f2)
  if [ "${py_ver%%.*}" -gt 2 ]; then
    execcmd python "$plot_script" "$data"
  else
    cd - >/dev/null
    die "error: Python3 is required"
  fi
else
  cd - >/dev/null
  die "error: Python is not availble on the system"
fi
[ $((dryrun)) -ne 0 ] || printf "%s\n" " Done."
cd - >/dev/null
