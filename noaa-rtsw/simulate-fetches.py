#!/usr/bin/env -S uv run --script
"""
Simulate making multiple 1-hour fetches over time using the 7-day file.
(In order to use them for the dashboard and prediction tasks.)
"""
# /// script
# dependencies = [
#   "numpy",
#   "pandas ~=2.2",
#   "pyarrow",
#   "requests",
# ]
# ///

import pandas as pd

from rtsw import get, save


df = get(hour=False)

tmin = df["time_tag"].min()
tmax = df["time_tag"].max()
times = pd.date_range(tmin.floor("h"), tmax, freq="1h")

for a, b in zip(times[:-1], times[1:]):
    df_ab = df[df.time_tag.between(a, b, inclusive="left")].reset_index()
    df_ab.attrs["retrieved_at"] = str(b)
    p = save(df_ab)
    print(p.as_posix())
