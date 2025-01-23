#!/usr/bin/env python3
"""Phase portrait plotter.

Examples
--------
Create a phase portrait:
$ (cd .. && make ppp_test)
$ ../ppp_test regulator >regulator.csv
$ uv run python plot_phase_portrait.py regulator.csv --xlim --ylim

Plot specific tags on the phase space:
$ (cd .. && make raibert_test)
$ ../raibert_test 1 >raibert.1.csv
$ uv run python plot_phase_portrait.py raibert.1.csv --tags 0 1

"""

from __future__ import annotations

import argparse
import sys
import warnings
from typing import TYPE_CHECKING, Literal

import matplotlib.pyplot as plt
from pyplotutil.datautil import TaggedData
from pyplotutil.loggingutil import evlog, get_event_logger_filename, start_logging
from pyplotutil.plotutil import get_limits

if TYPE_CHECKING:
    from matplotlib.axes import Axes


def list_tags(dataset: TaggedData) -> None:
    sys.stdout.write("index\ttag\n")
    sys.stdout.write("------------------\n")
    for i, tag in enumerate(sorted(dataset.tags())):
        sys.stdout.write(f"{i}\t{tag}\n")
    sys.stdout.flush()


def plot_params(ax: Axes, dataset: TaggedData, tags: list[str] | None) -> tuple[float, float, float]:
    if tags is None:
        zh, za, zb = dataset.param(["zh", "za", "zb"])
    else:
        if len(tags) == 0:
            msg = "No tag is specified. Nothing to plot."
            evlog().warning(msg)
            warnings.warn(msg, stacklevel=2)
            return 0.0, 0.0, 0.0
        zh, za, zb = dataset.param(["zh", "za", "zb"], tags[0])
    ax.axvline(zh, ls="-", lw=1, c="k")
    ax.axhline(0, ls="-", lw=1, c="k")
    ax.axvline(za, ls="--", lw=1, c="k")
    ax.axvline(zb, ls="--", lw=1, c="k")
    return zh, za, zb


def plot_all(ax: Axes, dataset: TaggedData) -> Axes:
    # Plot solution curves.
    for tag, data in dataset:
        ax.plot(data.z, data.vz, ls="-", lw=0.5, c="k", label=tag)
    return ax


def plot_tags(ax: Axes, dataset: TaggedData, tags: list[str]) -> Axes:
    # Plot solution curves of specified tags.
    for tag in tags:
        data = dataset.get(tag)
        ax.plot(data.z, data.vz, ls="-", lw=0.5, label=tag)
    return ax


def set_misc(
    ax: Axes,
    xlabel: str,
    ylabel: str,
    title: str | None,
    xlim: tuple[float, float] | None,
    ylim: tuple[float, float] | None,
    *,
    grid: bool | Literal["both", "x", "y"],
) -> Axes:
    ax.set_xlabel(xlabel)
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
    return ax


def plot(
    dataset: TaggedData,
    tags: list[str] | None,
    *,
    xlim: list[float] | tuple[float, float] | None,
    ylim: list[float] | tuple[float, float] | None,
    grid: bool | Literal["both", "x", "y"],
    show_legend: bool,
) -> None:
    if tags is not None:
        all_tags = sorted(dataset.tags())
        tags = [all_tags[int(tag)] if tag.isdigit() else tag for tag in tags]
    # Prepare figure.
    _, ax = plt.subplots(figsize=(12, 9))

    # Plot constant parameters.
    zh, za, zb = plot_params(ax, dataset, tags)

    # Plot solution curves.
    if tags is None:
        plot_all(ax, dataset)
    else:
        plot_tags(ax, dataset, tags)

    # Set misc.
    title = None
    if dataset.is_loaded_from_file():
        title = dataset.datapath.stem
    default_xlim = (dataset.param("xmin"), dataset.param("xmax"))
    default_ylim = (dataset.param("ymin"), dataset.param("ymax"))
    xlim, ylim = get_limits(xlim, ylim, fallback_xlim=default_xlim, fallback_ylim=default_ylim)
    set_misc(ax, "position [m]", "velocity [m/s]", title, xlim, ylim, grid=grid)

    if show_legend:
        ax.legend()


def parse() -> argparse.Namespace:
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(description="Plot solution curves on the phase space.")
    parser.add_argument("data", help="Path to a CSV file.")
    parser.add_argument(
        "-t",
        "--tags",
        nargs="+",
        help="Limit tags or indices to be shown in the graph.",
    )
    parser.add_argument(
        "--xlim",
        nargs="*",
        type=float,
        help="Limits along x-axis.",
    )
    parser.add_argument(
        "--ylim",
        nargs="*",
        type=float,
        help="Limits along y-axis.",
    )
    parser.add_argument(
        "--list-tags",
        action="store_true",
        help="List tags in the loaded data file and exit.",
    )
    parser.add_argument(
        "--grid",
        choices=["both", "x", "y"],
        default="both",
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


N_TAGS_SHOW_LEGEND = 7


def main() -> None:
    """Run main function."""
    args = parse()
    start_logging(sys.argv, None, __name__, args.verbose)
    evlog().info("Log filename: %s", get_event_logger_filename())

    dataset = TaggedData(args.data)
    if args.list_tags:
        list_tags(dataset)
        sys.exit()

    show_legend: bool | None = args.show_legend
    if show_legend is None and args.tags is not None and len(args.tags) <= N_TAGS_SHOW_LEGEND:
        show_legend = True

    plot(dataset, args.tags, xlim=args.xlim, ylim=args.ylim, grid=args.grid, show_legend=bool(show_legend))
    if args.show_plot:
        plt.show()


if __name__ == "__main__":
    main()

# Local Variables:
# jinx-local-words: "cd csv dat env ppp py raibert usr uv vv xlim xmax xmin ylim ymax ymin za zb zh"
# End:
