# -*- coding: utf-8 -*-

from pathlib import Path
import typing
from typing import cast

import pandas as pd
from pandas.io.parsers import TextFileReader
from pandas.api.types import is_string_dtype


class Data(object):
    @typing.overload
    def __init__(self, data: str, **kwargs) -> None:
        ...

    @typing.overload
    def __init__(self, data: pd.DataFrame, **kwargs) -> None:
        ...

    def __init__(self, data, **kwargs) -> None:
        self._csvpath: Path | None = None
        self._dataframe: pd.DataFrame | TextFileReader | None = None

        if isinstance(data, str):
            self.read_csv(data, **kwargs)
        elif isinstance(data, pd.DataFrame):
            self.copy_dataframe(data)
        else:
            raise RuntimeError("Data only accepts string or pandas.DataFrame")

    @typing.overload
    def __getitem__(self, key: int) -> pd.Series:
        ...

    @typing.overload
    def __getitem__(self, key: str) -> pd.Series:
        ...

    def __getitem__(self, key) -> pd.Series:
        if isinstance(self._dataframe, pd.DataFrame):
            return self._dataframe[key]
        else:
            raise RuntimeError("Data has not been constructed yet")

    def _setattr(self, dataframe: pd.DataFrame):
        if is_string_dtype(dataframe.columns):
            for c in dataframe.columns:
                col = cast(str, c)
                setattr(self, col, getattr(dataframe, col))

    @property
    def csvpath(self) -> str | None:
        if isinstance(self._csvpath, Path):
            return str(self._csvpath.resolve())

    @csvpath.setter
    def csvpath(self, path: str) -> None:
        if Path(path).is_file():
            self._csvpath = Path(path)
        else:
            raise FileNotFoundError(path)

    @property
    def dataframe(self) -> pd.DataFrame | None:
        if isinstance(self._dataframe, pd.DataFrame):
            return self._dataframe

    def read_csv(self, path: str, **kwargs) -> None:
        self.csvpath = path
        self._dataframe = pd.read_csv(self.csvpath, **kwargs)
        if isinstance(self._dataframe, pd.DataFrame):
            self._setattr(self._dataframe)
        else:
            raise RuntimeError("Unable to set attributes to Data")

    def copy_dataframe(self, dataframe: pd.DataFrame) -> None:
        self._dataframe = dataframe
        self._setattr(self._dataframe)
