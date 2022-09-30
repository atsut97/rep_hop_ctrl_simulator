#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import argparse
import warnings
from pathlib import Path

import matplotlib.pyplot as plt
from pyplotutil.datautil import DataSet

KNOWN_UNITS = {
    "position": "m",
    "velocity": "m/s",
    "acceleration": "m/s^2",
    "force": "N",
    "time": "s",
}


def load(datapath: str | Path) -> DataSet:
    dataset = DataSet(datapath)
    return dataset


def find_unit(label: str) -> str | None:
    return KNOWN_UNITS.get(label, None)


def parse_variables(given_vars: list[list[str]] | None) -> list[list[str | None]]:
    if given_vars is None:
        return []

    parsed_vars = []
    for given_var in given_vars:
        symbol, label, unit = None, None, None
        if len(given_var) == 0:
            continue
        symbol = given_var[0]
        if len(given_var) > 1:
            label = given_var[1]
            if len(given_var) > 2:
                unit = given_var[2]
        parsed_vars.append([symbol, label, unit])
    return parsed_vars


def parse_tags(given_tags: list[str] | None, dataset: DataSet) -> list[str]:
    if given_tags is None:
        return []

    parsed_tags = []
    if len(given_tags) == 1:
        # No tags specified
        parsed_tags.append(dataset.tags()[0])
    else:
        for tag in given_tags:
            if tag.isdigit():
                parsed_tags.append(dataset.tags()[int(tag)])
            else:
                parsed_tags.append(tag)
    return parsed_tags


def close_with_brackets(unit: str) -> str:
    if not unit.startswith("[") or not unit.endswith("]"):
        return f"[{unit}]"
    else:
        return unit


def get_label_with_unit(label: str | None, unit: str | None = None) -> str:
    if label is None:
        return ""
    if unit is None:
        unit = find_unit(label)
    if unit is not None:
        return f"{label} {close_with_brackets(unit)}"
    else:
        return label


def plot(dataset: DataSet, variables: list[list[str | None]], tags: list[str]):
    if len(variables) == 0:
        warnings.warn("No variables to plot specified", UserWarning)
        return

    # Prepare figure.
    n_ax = len(variables)
    fig = plt.figure(figsize=(10, 4 * n_ax))
    for i, (symbol, label, unit) in enumerate(variables):
        ax = fig.add_subplot(n_ax, 1, i + 1)
        for tag in tags:
            data = dataset.get(tag)
            if symbol is not None:
                ax.plot(data.t, getattr(data, symbol), label=tag)
        ax.set_ylabel(get_label_with_unit(label, unit))
        ax.grid()
        ax.legend()
    plt.gca().set_xlabel(get_label_with_unit("time"))


def show_symbols(dataset: DataSet):
    print(list(dataset.get().dataframe.columns))


def show_tags(dataset: DataSet):
    print(dataset.tags())


def parse():
    parser = argparse.ArgumentParser(
        description="Plot timeseries of specified variables."
    )
    parser.add_argument("datapath", help="Path to CSV file.")
    parser.add_argument(
        "-v",
        "--variable",
        action="append",
        nargs="+",
        required=True,
        help="Specify variable to be shown. It consists of <symbol> [<label> [<unit>]].",
    )
    parser.add_argument(
        "-t",
        "--tags",
        nargs="+",
        default=["0"],
        help="Specify tag or index to be shown.",
    )
    parser.add_argument(
        "--show-symbols",
        action="store_true",
        help="Show symbols found in data file and exit.",
    )
    parser.add_argument(
        "--show-tags",
        action="store_true",
        help="Show tags found in data file and exit.",
    )
    return parser.parse_args()


def main():
    args = parse()
    dataset = load(args.datapath)
    if args.show_symbols:
        show_symbols(dataset)
        exit()
    if args.show_tags:
        show_tags(dataset)
        exit()
    variables = parse_variables(args.variable)
    tags = parse_tags(args.tags, dataset)
    plot(dataset, variables, tags)
    plt.show()


if __name__ == "__main__":
    main()
