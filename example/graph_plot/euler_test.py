#!/usr/bin/env python3
"""Graph plotter for euler_test / rk4_test.

Examples
--------
Euler method:
$ (cd .. && make euler_test)
$ ../euler_test >euler.dat
$ uv run python euler_test.py euler.dat

Runge-Kutta 4th-order method:
$ (cd .. && make rk4_test)
$ ../rk4_test >rk4.dat
$ uv run python rk4_test.py rk4.dat

"""

from __future__ import annotations

from pathlib import Path

import matplotlib.pyplot as plt
from pyplotutil.datautil import Data


def plot(data: Data) -> None:
    plt.plot(data[:, 0], data[:, 1])
    plt.gca().set_aspect("equal", "box")
    plt.title(Path(__file__).stem)


def main() -> None:
    import sys

    data = Data(sys.argv[1], separator=" ", columns=[0, 1], has_header=False)
    plot(data)
    plt.show()


if __name__ == "__main__":
    main()

# Local Variables:
# jinx-local-words: "Kutta Runge cd dat env euler py usr uv"
# End:
