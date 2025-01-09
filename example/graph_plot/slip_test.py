#!/usr/bin/env python3
"""Graph plotter for slip_test / slip_var_stiff_test.

Examples
--------
SLIP model:
$ (cd .. && make slip_test)
$ ../slip_test >slip.dat
$ uv run python slip_test.py slip.dat

Variable stiffness SLIP model:
$ (cd .. && make slip_var_stiff_test)
$ ../slip_var_stiff_test >slip_var_stiff.dat
$ uv run python slip_var_stiff_test.py slip_var_stiff.dat

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
