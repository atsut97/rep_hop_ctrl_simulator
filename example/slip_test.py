#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import pandas as pd


def plot(data):
    plt.plot(data.z, data.v)


def main():
    import sys
    data = pd.read_csv(sys.argv[1])
    plot(data)
    plt.show()


if __name__ == '__main__':
    main()
