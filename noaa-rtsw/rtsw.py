"""
Load and display NOAA RTSW data
"""
# /// script
# dependencies = [
#   "matplotlib ~=3.10.3",
#   "numpy",
#   "pandas ~=2.2",
#   "requests",
# ]
# ///

import matplotlib.pyplot as plt
import pandas as pd
import requests

URL = (
    "https://services.swpc.noaa.gov/products/geospace/propagated-solar-wind-1-hour.json"
)

r = requests.get(URL)
r.raise_for_status()

# List of entries, which are lists
# The first entry is the column names
data = r.json()
df = pd.DataFrame(data[1:], columns=data[0])

# Everything should be numeric except the two time columns
for col in df:
    if "time_tag" in col:
        df[col] = pd.to_datetime(df[col])
    else:
        df[col] = pd.to_numeric(df[col], errors="coerce")

# Plot a time series
# Normal resolution seems to be 1 minute
# but sometimes time are missing
# We need consistent interval to get the nice pandas plot
col, label = "speed", "speed [km/s]"
s = df.set_index("time_tag")[col].resample("1min").mean()
s.plot(
    xlabel="UTC",
    ylabel=label,
    title="NOAA RTSW data",
    legend=False,
    figsize=(6, 4),
)
plt.tight_layout()

plt.show()
