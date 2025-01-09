#!/usr/bin/env python3
"""Graph plotter for mtoka_osci_test.

Examples
--------
$ (cd .. && make mtoka_osci_test)
$ ../mtoka_osci_test >mtoka_osci.dat
$ uv run python mtoka_osci_test.py mtoka_osci.dat

"""

from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
from pyplotutil.datautil import Data


def plot(data: Data) -> None:
    fig = plt.figure(figsize=(8, 10))
    ax1 = fig.add_subplot(411)
    ax1.plot(data.t, data.x1, label="x1")
    ax1.plot(data.t, data.v1, label="v1")
    ax1.plot(data.t, data.y1, label="y1")
    ax1.grid()
    ax1.legend()
    ax1.set_title(Path(__file__).stem)

    ax2 = fig.add_subplot(412)
    ax2.plot(data.t, data.x2, label="x2")
    ax2.plot(data.t, data.v2, label="v2")
    ax2.plot(data.t, data.y2, label="y2")
    ax2.grid()
    ax2.legend()

    ax3 = fig.add_subplot(413)
    ax3.plot(data.t, data.x1 - data.x2, label="x1-x2")
    ax3.plot(data.t, data.v1 - data.v2, label="v1-v2")
    ax3.plot(data.t, data.y1 - data.y2, label="y1-y2")
    ax3.grid()
    ax3.legend()

    ax4 = fig.add_subplot(414)
    ax4.plot(data.t, data.x1 + data.x2, label="x1+x2")
    ax4.plot(data.t, data.v1 + data.v2, label="v1+v2")
    ax4.plot(data.t, data.y1 + data.y2, label="y1+y2")
    ax4.grid()
    ax4.legend()


def main() -> None:
    import sys

    data = Data(sys.argv[1])
    plot(data)
    plt.show()


if __name__ == "__main__":
    main()

# Local Variables:
# jinx-local-words: "cd dat env mtoka osci py usr uv"
# End:
