#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
from pyplotutil.datautil import DataSet


def plot(dataset: DataSet) -> None:
    # Prepare figure.
    _, ax = plt.subplots()
    ax.set_xlabel("position [m]")
    ax.set_ylabel("velocity [m/s]")
    if dataset.datapath:
        ax.set_title(dataset.datapath.name)

    # Plot constant parameters.
    z0, zd, zb = dataset.param(["z0", "zd", "zb"])
    ax.axvline(z0, ls="-", lw=1, c="k")
    ax.axhline(0, ls="-", lw=1, c="k")
    ax.axvline(zd, ls="--", lw=1, c="k")
    ax.axvline(zb, ls="--", lw=1, c="k")

    # Plot solution curves.
    for data in dataset:
        ax.plot(data.z, data.vz, ls="-", lw=1, c="k")

    # Set misc.
    ax.grid()
    ax.set_xlim([zb - 0.01, zd + 0.01])
    ax.set_ylim([-0.8, 0.8])


def main() -> None:
    import sys

    if len(sys.argv) == 1:
        usage = f"""\
Usage:
  {sys.argv[0]} <data.csv>
"""
        print(usage)
        exit()

    dataset = DataSet(sys.argv[1])
    plot(dataset)
    plt.show()


if __name__ == "__main__":
    main()
