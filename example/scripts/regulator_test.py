#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt

from lib.datautil import DataGroup


def plot(datagroup):
    data = datagroup.get("sc00005")
    z0, zd, zb = data.param(["z0", "zd", "zb"])
    plt.axvline(z0, lw=1, c="k")
    plt.axhline(0, lw=1, c="k")
    plt.xlim([zb - 0.01, zd + 0.01])
    plt.ylim([-0.8, 0.8])
    plt.plot(data.z, data.vz)


def main():
    import sys

    datagroup = DataGroup(sys.argv[1])
    plot(datagroup)

    plt.show()


if __name__ == "__main__":
    main()
