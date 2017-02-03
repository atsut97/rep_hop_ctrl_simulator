#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np


def plot(data):
    plt.plot(data[:, 0], data[:, 1])


def main():
    import sys
    data = np.loadtxt(sys.argv[1])
    plot(data)
    plt.show()


if __name__ == '__main__':
    main()
