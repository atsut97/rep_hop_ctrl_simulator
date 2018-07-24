#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import pandas as pd


class Data(object):

    def __init__(self, df_or_filename):
        if isinstance(df_or_filename, pd.DataFrame):
            self.data = df_or_filename
            self.filename = None
        else:
            self.data = pd.read_csv(df_or_filename)
            self.filename = df_or_filename

    def getData(self):
        return self.data

    def hasFilename(self):
        return self.filename is not None

    def getFilename(self):
        return self.filename

    @property
    def z(self):
        return self.data.z

    @property
    def v(self):
        return self.data.v

    @property
    def fz(self):
        return self.data.fz

    @property
    def fe(self):
        return self.data.fe

    @property
    def zd(self):
        return self.data.zd

    @property
    def z0(self):
        return self.data.z0

    @property
    def zb(self):
        return self.data.zb


def plot(data):
    # prepare figure
    fig, ax = plt.subplots()
    if data.hasFilename():
        ax.set_title(data.getFilename())
    ax.set_xlabel('position [m]')
    ax.set_ylabel('velocity [m/s]')
    # plot parameters
    ax.axvline(data.z0[0], ls='-', lw=1, c='k')
    ax.axhline(0, ls='-', lw=1, c='k')
    ax.axvline(data.zd[0], ls='--', lw=1, c='k')
    ax.axvline(data.zb[0], ls='--', lw=1, c='k')
    # plot one hop
    ax.plot(data.z, data.v, ls='-', lw=1, c='k')
    ax.grid()


def main():
    import sys
    data = Data(sys.argv[1])
    plot(data)
    plt.show()


if __name__ == '__main__':
    main()
