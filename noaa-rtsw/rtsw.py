#!/usr/bin/env -S uv run --script
"""
Load and display NOAA RTSW data
"""
# /// script
# requires-python = ">=3.9"
# dependencies = [
#   "matplotlib ~=3.10.3",
#   "numpy",
#   "pandas ~=2.2",
#   "pyarrow",
#   "requests",
# ]
# ///

from __future__ import annotations

from pathlib import Path
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    import matplotlib as mpl
    import pandas as pd

HERE = Path(__file__).parent
FETCHES = HERE / "fetches"

BASE = "https://services.swpc.noaa.gov/products/geospace/"
URL = f"{BASE}/propagated-solar-wind.json"
URL_1H = f"{BASE}/propagated-solar-wind-1-hour.json"


def get(*, hour: bool = True) -> pd.DataFrame:
    import pandas as pd
    import requests

    r = requests.get(URL_1H if hour else URL)
    r.raise_for_status()

    # List of entries, which are themselves lists
    # The first entry is the column names
    data = r.json()
    df = pd.DataFrame(data[1:], columns=data[0])

    # Everything should be numeric except the two time columns
    for col in df:
        if "time_tag" in col:
            df[col] = pd.to_datetime(df[col])
        else:
            df[col] = pd.to_numeric(df[col], errors="coerce")

    df.attrs.update(
        retrieved_at=str(pd.Timestamp.now("UTC")),
    )

    return df


def plot(
    df: pd.DataFrame,
    *,
    column: str = "speed",
    label: str = "speed [km/s]",
) -> mpl.axes.Axes:
    """
    Plot the data in the DataFrame
    """
    # Normal resolution seems to be 1 minute
    # but sometimes times are missing
    # We need consistent interval to get the nice pandas plot
    s = df.set_index("time_tag")[column].resample("1min").mean()
    ax = s.plot(
        xlabel="UTC",
        ylabel=label,
        title="NOAA RTSW data",
        legend=False,
        figsize=(6, 4),
    )
    ax.set_title(
        pd.Timestamp(df.attrs["retrieved_at"]).strftime(r"%Y-%m-%d %H:%M:%S UTC"),
        loc="right",
        size=8,
    )
    ax.get_figure().tight_layout()

    return ax


def save(df: pd.DataFrame) -> Path:
    import pandas as pd

    FETCHES.mkdir(exist_ok=True, parents=True)

    t = pd.Timestamp(df.attrs["retrieved_at"])
    p = FETCHES / f"noaa-rtsw-{t:%Y%m%dT%H%M%S}.parquet"
    df.to_parquet(p)

    return p


def main() -> int:
    import argparse

    parser = argparse.ArgumentParser(
        description="Fetch and optionally plot NOAA RTSW data.",
    )
    parser.add_argument(
        "--hour",
        default=True,
        action=argparse.BooleanOptionalAction,
        help="Fetch the last hour of data (otherwise the last week or so).",
    )
    parser.add_argument(
        "--save",
        default=True,
        action=argparse.BooleanOptionalAction,
        help="Save the data to Parquet.",
    )
    parser.add_argument(
        "--show",
        default=False,
        action=argparse.BooleanOptionalAction,
        help="Show the plot",
    )
    args = parser.parse_args()

    df = get(hour=args.hour)

    if args.save:
        _ = save(df)

    if args.show:
        import matplotlib.pyplot as plt

        _ = plot(df)
        plt.show()

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
