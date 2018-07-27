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
            self.dataframe = pd.read_csv(df_or_filename, dtype={'tag': object})
            self.filename = df_or_filename
        self.makeDataList()

    def makeDataList(self):
        self.datadict = {}
        self.grouped = None

        if 'tag' in self.dataframe.columns:
            grouped = self.dataframe.groupby('tag')
            tags = grouped.groups.keys()
            self.datadict = {i: Data(grouped.get_group(i)) for i in tags}
        else:
            self.datadict = {'00000': Data(self.dataframe)}

    def __iter__(self):
        return iter(self.datadict.values())

    def tags(self):
        return list(self.datadict.keys())

    def items(self):
        return list(self.datadict.items())

    def getDataFrame(self):
        return self.dataframe

    def getDataDict(self):
        return self.datadict

    def getData(self, tag):
        return self.datadict[tag]

    def hasFilename(self):
        return self.filename is not None

    def getFilename(self):
        return self.filename

    def getParam(self, key, tag=None, index=0):
        if tag is None:
            tag = list(self.datadict.keys())[0]
        return self.datadict[tag].getParam(key, index)

    def getParamList(self, key_list, tag=None, index=0):
        return [self.getParam(key, tag, index) for key in key_list]


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
    if len(sys.argv) == 1:
        print('Please specify csv data file')
        exit()
    dataset = DataSet(sys.argv[1])
    plot(dataset)
    plt.show()


if __name__ == '__main__':
    main()
