#!/bin/sh

usage() {
  cat <<USAGE
Run example program and make plot.

Synopsis:
  $0 [-d <file>|--data=<file>] [-p <file>|--plot-script=<file>]
     [-w <directory>|--work-directory=<directory>]
     [-s <directory>|--scripts-directory=<directory>]
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

  -w <directory>, --work-directory=<directory>
    Change to <directory> when running make, program and script. It is
    set to the current working directory by default.

  -s <directory>, --scripts-directory=<directory>
    Specify a directory path that contains scripts for plotting. The
    default value is where this script ($0) exists.

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
startdir=$PWD
workdir=''
scriptsdir=''
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
    -w|--work-directory|--working-directory)
      if [ -n "$2" ] && echo "$2" | grep -q -- '^[^-]'; then
        workdir="$2"
        shift
      else
        die "error: --work-directory reauires a non-empty argument"
      fi
      ;;
    --work-directory=?*|--working-directory=?*)
      workdir="${1#*=}"
      ;;
    '--work-directory='|'--working-directory=')
      die "error: --work-directory reauires a non-empty argument"
      ;;
    -s|--scripts-directory|--script-directory)
      if [ -n "$2" ] && echo "$2" | grep -q -- '^[^-]'; then
        scriptsdir="$2"
        shift
      else
        die "error: --scripts-directory reauires a non-empty argument"
      fi
      ;;
    --scripts-directory=?*|--script-directory=?*)
      scriptsdir="${1#*=}"
      ;;
    '--scripts-directory='|'--script-directory=')
      die "error: --scripts-directory reauires a non-empty argument"
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
  if [ -n "$workdir" ]; then
    cd "$startdir" || exit 1
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

# Change directory if needed.
if [ -n "$workdir" ]; then
  cd "$workdir" || exit 1
else
  workdir=$startdir
fi

# Run make.
if [ $((make)) -ne 0 ]; then
  execcmd make
fi

# Run program and redirect produced output to an intermediate data
# file.
program="$1"; shift
case "$program" in
  /*|./*) ;;
  # Fix program path.
  *) program=./$program ;;
esac
program_name=$(basename "$program")
if [ $((run)) -ne 0 ]; then
  ret=0
  if [ -f "$program" ]; then
    [ -z "$data" ] && data="${program_name}.csv"
    if [ $((dryrun)) -ne 0 ]; then
      execcmd "$program" "$@" '>' "$data"
    else
      printf "%s" "Making plot data with $program_name ..."
      execcmd "$program" "$@" > "$data"
      ret=$?
      printf "%s\n" " Done."
    fi
  else
    die "error: ${program} not found in ${PWD}"
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

# Print the given path when it is accessible and written in Python
# with the file command. When the file command is unavailable on the
# system always print the given path if it exists.
_is_python_script() {
  given_path=$1
  if [ -f "$given_path" ]; then
    if ! command -v file >/dev/null || \
        file "$given_path" | grep -q "Python script"; then
      echo "$given_path"
    fi
  fi
  unset given_path
}

# Try to find a plot script in $scriptdir, whose filename is given as
# the 1st argument.
find_plot_script() {
  name=$1
  found_path=$(_is_python_script "$name")
  [ -z "$found_path" ] && found_path=$(_is_python_script "${scriptsdir}/${name}.py")
  [ -z "$found_path" ] && found_path=$(_is_python_script "${scriptsdir}/${name}")
  echo "$found_path"
  unset name found_path
}

# Plot the data with an appropriate Python script.
if [ -z "$scriptsdir" ]; then
  scriptsdir=$(cd -- "$(dirname -- "$0")" && pwd -P)/scripts
fi
if [ -n "$plot_script" ]; then
  # Fix path to the plot script specified by the user.
  _plot_script=$(find_plot_script "$plot_script")
  [ -z "$_plot_script" ] && die "error: ${plot_script} not found in ${scriptsdir}"
else
  # Find the plot script from the program name.
  _plot_script=$(find_plot_script "$program_name")
  [ -z "$_plot_script" ] && die <<EOF
error: Unable to find plot script for ${program_name} in ${scriptsdir}
Try to use plot_one_hop.py like as:
  $0 -p plot_one_hop slip_test
EOF
fi
plot_script=$_plot_script

[ $((dryrun)) -ne 0 ] || printf "%s" "Running script to plot data ..."
if command -v poetry >/dev/null && \
    [ -f "${scriptsdir}/pyproject.toml" ]; then
  (
    cd "$scriptsdir" || exit 1
    execcmd poetry run sh -c "cd \"$workdir\" && python \"$plot_script\" \"$data\""
  )
elif command -v pipenv >/dev/null && \
    [ -f "${scriptsdir}/Pipfile" ]; then
  (
    cd "$scriptsdir" || exit 1
    execcmd pipenv sh -c "cd \"$workdir\" && python \"$plot_script\" \"$data\""
  )
elif command -v python >/dev/null; then
  py_ver=$(python --version | cut -d' ' -f2)
  if [ "${py_ver%%.*}" -gt 2 ]; then
    execcmd python "$plot_script" "$data"
  else
    die "error: Python3 is required"
  fi
else
  die "error: Python is not availble on the system"
fi
[ $((dryrun)) -ne 0 ] || printf "%s\n" " Done."
