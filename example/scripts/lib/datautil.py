# -*- coding: utf-8 -*-

from abc import ABC
from collections.abc import Iterator
from pathlib import Path
from typing import Any, Tuple, overload

import numpy as np
import pandas as pd
from pandas.api.types import is_string_dtype

NumericType = int | float | complex | np.number


class DataBase(ABC):
    _csvpath: Path
    _dataframe: pd.DataFrame

    def _get_csvpath(self) -> Path:
        return self._csvpath

    def _set_csvpath(self, path: Path | str) -> None:
        self._csvpath = Path(path)

    csvpath = property(_get_csvpath, _set_csvpath)

    def _get_dataframe(self) -> pd.DataFrame:
        return self._dataframe

    dataframe = property(_get_dataframe)


class Data(DataBase):
    @overload
    def __init__(self, data: str | Path, **kwargs) -> None:
        ...

    @overload
    def __init__(self, data: pd.DataFrame | pd.Series, **kwargs) -> None:
        ...

    def __init__(self, data, **kwargs) -> None:
        if isinstance(data, (str, Path)):
            self.read_csv(data, **kwargs)
        elif isinstance(data, (pd.DataFrame, pd.Series)):
            self.copy_dataframe(data)
        else:
            raise TypeError(f"unsupported type: {type(data)}")

    def __getitem__(self, key: int | str) -> pd.Series:
        return self._dataframe[key]

    def _set_attr(self) -> None:
        if is_string_dtype(self._dataframe.columns):
            for c in self._dataframe.columns:
                setattr(self, str(c), getattr(self._dataframe, str(c)))

    def read_csv(self, path: str | Path, **kwargs) -> None:
        self.csvpath = path
        df = pd.read_csv(self.csvpath, **kwargs)
        if isinstance(df, pd.DataFrame):
            self._dataframe = df
        else:
            raise TypeError(f"unsupported type: {type(df)}")
        self._set_attr()

    def copy_dataframe(self, df: pd.DataFrame | pd.Series) -> None:
        if isinstance(df, pd.DataFrame):
            self._dataframe = df
        else:
            self._dataframe = pd.DataFrame(df)
        self._set_attr()

    @overload
    def param(self, col: str) -> NumericType:
        ...

    @overload
    def param(self, col: list[str] | tuple[str]) -> list[NumericType]:
        ...

    def param(self, col):
        if isinstance(col, str):
            return self.dataframe.at[0, col]
        else:
            return [self.dataframe.at[0, c] for c in col]


class DataGroup(DataBase):
    _datadict: dict[str, Data]
    _groupby: Any
    _by: str

    def __init__(self, data: str | Path, **kwargs) -> None:
        self._datadict = {}
        self._by = kwargs.pop("by", "tag")

        if isinstance(data, (str, Path)):
            self.read_csv(data, **kwargs)
            self.make_groupby()
        else:
            raise TypeError(f"unsupported type: {type(data)}")

    @property
    def datadict(self) -> dict[str, Data]:
        return self._datadict

    def read_csv(self, path: str | Path, **kwargs) -> None:
        self.csvpath = path
        if "dtype" not in kwargs:
            kwargs["dtype"] = {self._by: object}
        else:
            kwargs["dtype"].update({self._by: object})

        df = pd.read_csv(self.csvpath, **kwargs)
        if isinstance(df, pd.DataFrame):
            self._dataframe = df
        else:
            raise TypeError(f"unsupported type: {type(df)}")

    def make_groupby(self):
        if self._by in self._dataframe.columns:
            self._groupby = self._dataframe.groupby(self._by)
            self._datadict = {
                str(k): Data(self._groupby.get_group(k).reset_index(drop=True))
                for k in self._groupby.groups.keys()
            }
        else:
            self._datadict = {"0": Data(self._dataframe)}

    def __iter__(self) -> Iterator[Data]:
        return iter(self._datadict.values())

    def keys(self) -> list[str]:
        return list(self._datadict.keys())

    def items(self) -> list[Tuple[str, Data]]:
        return list(self._datadict.items())

    def param(
        self, col: str | list[str] | tuple[str], tag=None
    ) -> NumericType | list[NumericType]:
        if tag is None:
            tag = self.keys()[0]
        return self.datadict[tag].param(col)

    def get(self, tag: str = None) -> Data:
        if tag is None:
            tag = self.keys()[0]
        return self.datadict[tag]
