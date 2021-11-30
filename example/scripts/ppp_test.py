#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt

from lib.datautil import DataGroup


def plot(datagroup):
    # Prepare figure.
    _, ax = plt.subplots()
    ax.set_xlabel("position [m]")
    ax.set_ylabel("velocity [m/s]")
    if datagroup.csvpath:
        ax.set_title(datagroup.csvpath.name)

    # Plot constant parameters.
    z0, zd, zb = datagroup.param(["z0", "zd", "zb"])
    ax.axvline(z0, ls="-", lw=1, c="k")
    ax.axhline(0, ls="-", lw=1, c="k")
    ax.axvline(zd, ls="--", lw=1, c="k")
    ax.axvline(zb, ls="--", lw=1, c="k")

    # Plot solution curves.
    for data in datagroup:
        ax.plot(data.z, data.vz, ls="-", lw=1, c="k")

    # Set misc.
    ax.grid()
    ax.set_xlim([zb - 0.01, zd + 0.01])
    ax.set_ylim([-0.8, 0.8])


def main():
    import sys

    if len(sys.argv) == 1:
        usage = f"""\
Usage:
  {sys.argv[0]} <data.csv>
"""
        print(usage)
        exit()

    datagroup = DataGroup(sys.argv[1])
    plot(datagroup)
    plt.show()


if __name__ == "__main__":
    main()
