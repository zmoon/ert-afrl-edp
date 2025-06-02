#!/usr/bin/env -S uv run --script
"""
Fetch ACE solar wind data archive to use as training data.
"""
# /// script
# dependencies = [
#   "numpy",
#   "pandas ~=2.2",
#   "pyarrow",
#   "requests",
#   "stamina",
#   "tqdm",
# ]
# ///

import re
from pathlib import Path

import pandas as pd
import requests
from tqdm import tqdm

HERE = Path(__file__).parent
BASE = "https://sohoftp.nascom.nasa.gov/sdb/goes/ace/monthly/"

# First determine the files available
r = requests.get(BASE)
r.raise_for_status()

# For example, the latest file is '202505_ace_swepam_1h.txt'
files = re.findall(r'href="(\d{6}_ace_swepam_1h\.txt)"', r.text)
assert files

columns = [
    "year",
    "month",
    "day",
    "hhmm",
    "jday",
    "seconds",
    "status",
    "density",
    "speed",
    "temp",
]


dfs = []
for f in tqdm(files):
    this_df = pd.read_csv(
        BASE + f,
        sep=r"\s+",
        comment="#",
        skiprows=2,
        header=None,
        na_values=[-9999.9, -1e5],
    )
    assert len(this_df.columns) == len(columns)
    dfs.append(this_df)

# Combine
df = pd.concat(dfs, ignore_index=True)
df.columns = columns

# Convert to datetime
df = (
    df.assign(time=pd.to_datetime(df[["year", "month", "day", "seconds"]]))
    .drop(
        columns=[
            "year",
            "month",
            "day",
            "hhmm",
            "jday",
            "seconds",
        ]
    )
    .drop_duplicates(subset=["time"], keep="last")
    .set_index("time")
)

# Status is categorical
# "Status(S): 0 = nominal data, 1 to 8 = bad data record, 9 = no data"
df = df.assign(
    status=pd.Categorical(
        df["status"],
        categories=range(10),
        ordered=True,
    )
)

# Fill NaNs
to_interp = ["density", "speed", "temp"]
df[to_interp] = df[to_interp].interpolate(method="time")

# Checks
assert df.index.diff().to_series().iloc[1:].eq(pd.Timedelta("1h")).all()
assert df.notna().all().all()

# Save to Parquet
df.to_parquet(
    HERE / "ace-sw-archive.parquet",
    index=True,
)
