#!/usr/bin/env python3
"""Time series plotter.

Examples
--------
Create a phase portrait:
$ (cd .. && make regulator_test)
$ ../regulator_test >regulator.dat
$ uv run python plot_time_series.py regulator.dat

Plot specific tags on the phase space:
$ (cd .. && make raibert_test)
$ ../raibert_test 1 >raibert.1.dat
$ uv run python plot_time_series.py raibert.1.dat -s z position -s vz velocity -t 0 1

"""

from __future__ import annotations

import argparse
import sys
import warnings
from typing import TYPE_CHECKING, Literal

import matplotlib.pyplot as plt
import numpy as np
from pyplotutil.datautil import TaggedData
from pyplotutil.loggingutil import evlog, get_event_logger_filename, start_logging
from pyplotutil.plotutil import get_limits, get_tlim_mask

if TYPE_CHECKING:
    from pathlib import Path

    from matplotlib.axes import Axes

KNOWN_UNITS = {
    "position": "m",
    "velocity": "m/s",
    "acceleration": "m/s^2",
    "force": "N",
    "time": "s",
}


def list_tags(dataset: TaggedData) -> None:
    sys.stdout.write("index\ttag\n")
    sys.stdout.write("------------------\n")
    for i, tag in enumerate(sorted(dataset.tags())):
        sys.stdout.write(f"{i}\t{tag}\n")
    sys.stdout.flush()


def list_symbols(dataset: TaggedData) -> None:
    sys.stdout.write("symbol\n")
    sys.stdout.write("------\n")
    tag = sorted(dataset.tags())[0]
    for s in dataset.get(tag).columns:
        sys.stdout.write(f"{s}\n")
    sys.stdout.flush()


def load(datapath: str | Path) -> TaggedData:
    return TaggedData(datapath)


def find_unit(label: str) -> str | None:
    return KNOWN_UNITS.get(label)


def parse_symbols(given_symbols: list[list[str]] | None) -> list[tuple[str, str | None, str | None]]:
    if given_symbols is None:
        return []

    parsed_symbols: list[tuple[str, str | None, str | None]] = []
    for slu in given_symbols:
        symbol, label, unit = None, None, None
        if len(slu) == 0:
            continue
        symbol = slu.pop(0)
        if len(slu) > 0:
            label = slu.pop(0)
        if len(slu) > 0:
            unit = slu.pop(0)
        parsed_symbols.append((symbol, label, unit))
    return parsed_symbols


def parse_tags(given_tags: list[str] | None, dataset: TaggedData) -> list[str]:
    all_tags = sorted(dataset.tags())
    if given_tags is None:
        return [all_tags[0]]

    parsed_tags = [all_tags[int(tag)] if tag.isdigit() else tag for tag in given_tags]
    if len(parsed_tags) == 0:
        parsed_tags.append(all_tags[0])
    return parsed_tags


def close_with_brackets(unit: str) -> str:
    if not unit.startswith("[") or not unit.endswith("]"):
        return f"[{unit}]"
    return unit


def get_label_with_unit(label: str | None, unit: str | None = None) -> str:
    if label is None:
        return ""
    if unit is None:
        unit = find_unit(label)
    if unit is not None:
        return f"{label} {close_with_brackets(unit)}"
    return label


def set_misc(
    ax: Axes,
    xlabel: str | None,
    ylabel: str | None,
    title: str | None,
    xlim: tuple[float, float] | None,
    ylim: tuple[float, float] | None,
    *,
    grid: bool | Literal["both", "x", "y"],
    show_legend: bool,
) -> Axes:
    if xlabel:
        ax.set_xlabel(xlabel)
    if ylabel:
        ax.set_ylabel(ylabel)
    if title:
        ax.set_title(title)
    if xlim:
        ax.set_xlim(xlim)
    if ylim:
        ax.set_ylim(ylim)
    if grid:
        grid_axis: Literal["both", "x", "y"] = "both"
        if grid in ("both", "x", "y") and grid is not True:
            grid_axis = grid
        ax.grid(visible=True, axis=grid_axis)
    if show_legend:
        ax.legend()
    return ax


def plot(
    dataset: TaggedData,
    symbols: list[list[str]] | None,
    tags: list[str] | None,
    *,
    tlim: list[float] | tuple[float, float] | None,
    ylim: list[float] | tuple[float, float] | None,
    grid: bool | Literal["both", "x", "y"],
    show_legend: bool,
) -> None:
    parsed_symbols = parse_symbols(symbols)
    if len(parsed_symbols) == 0:
        msg = "No variables to plot specified"
        warnings.warn(msg, UserWarning, stacklevel=2)
        evlog().warning(msg)
        return

    # Prepare figure.
    n_axis = len(parsed_symbols)
    parsed_tags = parse_tags(tags, dataset)
    if tlim is not None and len(tlim) == 1:
        tlim = (0.0, tlim[0])
    tlim, ylim = get_limits(tlim, ylim)
    fig = plt.figure(figsize=(10, 4 * n_axis))
    for i, (symbol, label, unit) in enumerate(parsed_symbols):
        ax = fig.add_subplot(n_axis, 1, i + 1)
        for tag in parsed_tags:
            data = dataset.get(tag)
            t = np.ravel(data.t)
            y = np.ravel(data.get_column(symbol))
            mask = get_tlim_mask(t, tlim)
            ax.plot(t[mask], y[mask], label=tag)
        set_misc(ax, None, get_label_with_unit(label, unit), None, tlim, ylim, grid=grid, show_legend=show_legend)
    plt.gca().set_xlabel(get_label_with_unit("time"))


def parse() -> argparse.Namespace:
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(description="Plot time series of a specified symbol.")
    parser.add_argument("data", help="Path to a CSV file.")
    parser.add_argument(
        "-s",
        "--symbol",
        action="append",
        nargs="+",
        help="Specify symbol to be shown. It consists of <symbol> [<label> [<unit>]].",
    )
    parser.add_argument(
        "-t",
        "--tags",
        nargs="+",
        default=["0"],
        help="Specify tags or indices to be shown in the graph (default: 0).",
    )
    parser.add_argument(
        "--tlim",
        nargs="*",
        type=float,
        help="Limits along time axis.",
    )
    parser.add_argument(
        "--ylim",
        nargs="*",
        type=float,
        help="Limits along y-axis.",
    )
    parser.add_argument(
        "--list-symbols",
        action="store_true",
        help="List symbols in the loaded data file and exit.",
    )
    parser.add_argument(
        "--list-tags",
        action="store_true",
        help="List tags in the loaded data file and exit.",
    )
    parser.add_argument(
        "--grid",
        choices=["both", "x", "y"],
        default="y",
        help="Which axis to show grid lines.",
    )
    parser.add_argument(
        "--no-grid",
        dest="grid",
        action="store_false",
        help="Do not show grid lines.",
    )
    parser.add_argument(
        "--show-legend",
        action=argparse.BooleanOptionalAction,
        help="Whether to show a legend.",
    )
    parser.add_argument(
        "--show-plot",
        action=argparse.BooleanOptionalAction,
        default=True,
        help="Whether to show a plot window.",
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="count",
        default=0,
        help="Enable verbose output. -v provides additional info. -vv provides debug output.",
    )
    return parser.parse_args()


N_TAGS_SHOW_LEGEND = 5


def main() -> None:
    """Run main function."""
    args = parse()
    start_logging(sys.argv, None, __name__, args.verbose)
    evlog().info("Log filename: %s", get_event_logger_filename())

    dataset = load(args.data)
    if args.list_symbols:
        list_symbols(dataset)
        sys.exit()
    if args.list_tags:
        list_tags(dataset)
        sys.exit()

    show_legend = False
    if args.show_legend is None and args.tags is not None and len(args.tags) <= N_TAGS_SHOW_LEGEND:
        show_legend = True

    plot(dataset, args.symbol, args.tags, tlim=args.tlim, ylim=args.ylim, grid=args.grid, show_legend=show_legend)
    if args.show_plot:
        plt.show()


if __name__ == "__main__":
    main()

# Local Variables:
# jinx-local-words: "cd dat env py raibert tlim usr uv vv vz ylim"
# End:
