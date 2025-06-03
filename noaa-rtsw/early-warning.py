# /// script
# requires-python = ">=3.12"
# dependencies = [
#     "marimo",
#     "matplotlib==3.10.3",
#     "numpy==2.2.6",
#     "pandas==2.2.3",
#     "pyarrow==20.0.0",
# ]
# ///

import marimo

__generated_with = "0.13.15"
app = marimo.App(width="medium")


@app.cell
def _():
    from pathlib import Path

    import marimo as mo
    import matplotlib.pyplot as plt
    import numpy as np
    import pandas as pd
    from matplotlib.lines import Line2D

    HERE = Path(__file__).parent
    return HERE, Line2D, mo, np, pd, plt


@app.cell
def _(HERE, pd):
    df = (
        pd.concat(
            [pd.read_parquet(p) for p in (HERE / "fetches").glob("noaa-rtsw-*.parquet")]
        )
        .drop_duplicates(keep="last")
        .set_index("time_tag")
        .resample("1h")
        .mean()
    )
    df
    return (df,)


@app.cell
def _(mo):
    diff_roll_slider = mo.ui.slider(1, 10, value=3)
    mo.md(
        f"Choose smoothing window (hours) for solar wind cceleration plot: {diff_roll_slider}"
    )
    return (diff_roll_slider,)


@app.cell
def _(df, diff_roll_slider):
    def _():
        ax = df["speed"].plot(figsize=(8, 4), xlabel="UTC", ylabel="speed [km/s]")

        ax2 = ax.twinx()
        s = df["speed"].rolling(f"{diff_roll_slider.value}h", center=True).mean()
        dt = s.index.diff().to_series().dt.total_seconds() / 3600
        (s.diff() / dt.values).plot(ax=ax2, ls=":", c="C1")
        ax2.set_ylabel("acceleration [km/s/h]")

        ax.get_figure().tight_layout()

        return ax

    _()
    return


@app.cell
def _(mo):
    classify_arr = mo.ui.array(
        [
            mo.ui.slider(1, 10, value=3, label="window"),
            mo.ui.radio(
                options=["both", "left", "right", "neither"],
                value="both",
                label="closed",
            ),
        ]
    )

    mo.md(f"Control the assessment of sustain:\n{classify_arr}")
    return (classify_arr,)


@app.cell
def _(classify_arr, df, np, pd):
    # Classify data

    cuts = [500, 600, 800, np.inf]
    labels = ["moderate", "strong", "extreme"]
    colors = ["salmon", "tomato", "orangered"]
    linewidths = [1.8, 2.3, 3]

    df["strength"] = pd.cut(df["speed"], bins=cuts, labels=labels, right=False)

    def sustained(s):
        # NaN gets code -1
        return s.lt(0).sum() == 0 and s.nunique() == 1

    # Look for sustained regions
    # For a given point, the window can be left or right of it
    # Need to consider measurement window when flipping series (maybe shift)
    # window, closed = "3h", "both"
    window, closed = classify_arr.value
    if isinstance(window, int):
        window = f"{window}h"
    codes = df["strength"].cat.codes
    df["sustained"] = codes.rolling(window, closed=closed).apply(sustained).astype(
        bool
    ) | codes.iloc[::-1].rolling(window, closed=closed).apply(sustained).iloc[
        ::-1
    ].astype(bool)
    return colors, cuts, labels, linewidths


@app.cell
def _(Line2D, colors, cuts, df, labels, linewidths, np, plt):
    ax = (
        df["sustained"]
        .astype(int)
        .plot(figsize=(8, 4), label="sustained (1=yes)", c="C2")
    )
    df["strength"].cat.codes.astype(int).plot(
        ax=ax, label="strength code\n(0=moderate)", c="C0"
    )
    ax.set_xlabel("UTC")
    ax.legend(loc="upper left")

    ax2 = ax.twinx()
    df["speed"].plot(ax=ax2, c="0.25", ylabel="speed [km/s]")

    a, b = ax2.get_ylim()
    for x in cuts:
        if not np.isinf(x) and a < x < b:
            ax2.axhline(x, ls=":", c="0.5", zorder=0)

    for i, (color, cat, lw) in enumerate(zip(colors, labels, linewidths)):
        # We use `.where` instead of selecting the data
        # so that the lines won't be connected where they shouldn't
        df.where(df["strength"].eq(cat) & df["sustained"])["speed"].plot(
            c=color,
            ax=ax2,
            lw=lw,
        )

    handles = [
        Line2D([], [], color=color, lw=lw) for color, lw in zip(colors, linewidths)
    ]
    plt.legend(handles=handles, labels=labels, loc="lower right", title="strength")

    # Reset needed after colored plots
    ax.set_xlim(xmin=df.index.min())

    ax.get_figure().tight_layout()

    # So marimo will show it
    ax
    return


if __name__ == "__main__":
    app.run()
