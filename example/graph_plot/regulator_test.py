#!/usr/bin/env python3
"""Graph plotter for regulator_test.

Examples
--------
$ (cd .. && make regulator_test)
$ ../regulator_test >regulator.dat
$ uv run python regulator_test.py regulator.dat

"""

from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
from pyplotutil.datautil import TaggedData


def plot(dataset: TaggedData) -> None:
    data = dataset.get("sc00005")
    zh, za, zb = data.param(["zh", "za", "zb"])
    plt.axvline(zh, lw=1, c="k")
    plt.axhline(0, lw=1, c="k")
    plt.xlim([zb - 0.01, za + 0.01])
    plt.ylim([-0.8, 0.8])
    plt.plot(data.z, data.vz)
    plt.title(Path(__file__).stem)


def main() -> None:
    import sys

    dataset = TaggedData(sys.argv[1])
    plot(dataset)

    plt.show()


if __name__ == "__main__":
    main()

# Local Variables:
# jinx-local-words: "cd dat env py usr uv za zb zh"
# End:
