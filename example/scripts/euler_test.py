#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
from pyplotutil.datautil import Data


def plot(data: Data) -> None:
    plt.plot(data[0], data[1])


def main() -> None:
    import sys

    data = Data(sys.argv[1], sep=" ", usecols=[0, 1], header=None)
    plot(data)
    plt.show()


if __name__ == "__main__":
    main()
