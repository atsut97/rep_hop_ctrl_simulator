#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import pandas as pd


class Data(object):

    def __init__(self, df_or_filename):
        if isinstance(df_or_filename, pd.DataFrame):
            self.dataframe = df_or_filename
            self.filename = None
        else:
            self.dataframe = pd.read_csv(df_or_filename)
            self.filename = df_or_filename

        for c in self.dataframe.columns:
            setattr(self, c, getattr(self.dataframe, c))

    def getDataFrame(self):
        return self.dataframe

    def hasFilename(self):
        return self.filename is not None

    def getFilename(self):
        return self.filename

    def getParam(self, key, index=0):
        return self.dataframe[key][index]


class DataSet(object):

    def __init__(self, df_or_filename):
        if isinstance(df_or_filename, pd.DataFrame):
            self.dataframe = df_or_filename
            self.filename = None
        else:
            self.dataframe = pd.read_csv(df_or_filename)
            self.filename = df_or_filename
        self.makeDataList()

    def makeDataList(self):
        self.dataset = []
        self.grouped = None

        if 'id' in self.dataframe.columns:
            grouped = self.dataframe.groupby('id')
            ids = grouped.groups.keys()
            self.dataset = [Data(grouped.get_group(i)) for i in ids]
        else:
            self.dataset = [Data(self.dataframe)]

    def __iter__(self):
        return iter(self.dataset)

    def getDataFrame(self):
        return self.dataframe

    def getData(self, index):
        return self.dataset[index]

    def hasFilename(self):
        return self.filename is not None

    def getFilename(self):
        return self.filename

    def getParam(self, key, id=0, index=0):
        return self.dataset[id].getParam(key, index)

    def getParamList(self, key_list, id=0, index=0):
        return [self.getParam(key, id, index) for key in key_list]


def plot(dataset):
    # prepare figure
    fig, ax = plt.subplots()
    ax.set_xlabel('position [m]')
    ax.set_ylabel('velocity [m/s]')
    if dataset.hasFilename():
        ax.set_title(dataset.getFilename())

    # plot parameters
    z0, zd, zb = dataset.getParamList(['z0', 'zd', 'zb'])
    ax.axvline(z0, ls='-', lw=1, c='k')
    ax.axhline(0, ls='-', lw=1, c='k')
    ax.axvline(zd, ls='--', lw=1, c='k')
    ax.axvline(zb, ls='--', lw=1, c='k')

    # plot each hop
    for data in dataset:
        ax.plot(data.z, data.vz, ls='-', lw=1, c='k')

    # set misc.
    ax.grid()
    ax.set_xlim([zb-0.01, zd+0.01])
    ax.set_ylim([-0.8, 0.8])


def main():
    import sys
    dataset = DataSet(sys.argv[1])
    plot(dataset)
    plt.show()


if __name__ == '__main__':
    main()
