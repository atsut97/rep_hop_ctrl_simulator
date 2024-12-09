#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
from pyplotutil.datautil import DataSet


def plot(dataset: DataSet) -> None:
    data = dataset.get("sc00005")
    zh, za, zb = data.param(["zh", "za", "zb"])
    plt.axvline(zh, lw=1, c="k")  # type: ignore
    plt.axhline(0, lw=1, c="k")
    plt.xlim([zb - 0.01, za + 0.01])
    plt.ylim([-0.8, 0.8])
    plt.plot(data.z, data.vz)


def main() -> None:
    import sys

    dataset = DataSet(sys.argv[1])
    plot(dataset)

    plt.show()


if __name__ == "__main__":
    main()
