#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
from pyplotutil.datautil import DataSet


def load(argv: list[str]) -> tuple[DataSet, list[str]]:
    dataset = DataSet(argv[1])
    tags = []
    if len(argv) == 2:
        # No tags specified.
        tags.append(dataset.tags()[0])
    else:
        for tag in argv[2:]:
            if tag.isdigit():
                tags.append(dataset.tags()[int(tag)])
            else:
                tags.append(tag)
    return dataset, tags


def plot(dataset: DataSet, tags: list[str]) -> None:
    # Prepare figure.
    _, ax = plt.subplots()
    ax.set_xlabel("position [m]")
    ax.set_ylabel("velocity [m/s]")
    if dataset.datapath:
        ax.set_title(dataset.datapath.name)

    # Plot constant parameters.
    zh, za, zb = dataset.param(["zh", "za", "zb"])
    ax.axvline(zh, ls="-", lw=1, c="k")
    ax.axhline(0, ls="-", lw=1, c="k")
    ax.axvline(za, ls="--", lw=1, c="k")
    ax.axvline(zb, ls="--", lw=1, c="k")

    # Plot a specified trajectory.
    for tag in tags:
        data = dataset.get(tag)
        ax.plot(data.z, data.vz, lw=1, label=tag)

    # Set miscellaneous parameters.
    ax.grid()
    ax.legend()


def usage(prog) -> None:
    msg = f"""\
Usage:
    {prog} <data.csv> [index...]

Arguments:
    data.csv (required):
        CSV format data file.
    index (optional):
        Data index (integer) or tag string.
        Multiple indices or tags can be specified.
"""
    print(msg)


def parse(argv: list[str]) -> bool:
    if len(argv) == 1 or argv[1] in ["-h", "-help", "--help"]:
        usage(argv[0])
        return False

    return True


def main() -> None:
    import sys

    if not parse(sys.argv):
        exit(1)

    dataset, tag = load(sys.argv)
    plot(dataset, tag)
    plt.show()


if __name__ == "__main__":
    main()
