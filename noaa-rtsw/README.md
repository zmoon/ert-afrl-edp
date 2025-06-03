# NOAA real-time solar wind (RTSW) data

https://www.swpc.noaa.gov/products/real-time-solar-wind

Relevant data URLs:

- https://services.swpc.noaa.gov/products/geospace/propagated-solar-wind-1-hour.json
  (recent data, the URL shared in the problem description, 1-minute resolution)
- https://services.swpc.noaa.gov/products/geospace/propagated-solar-wind.json
  (recent 7 days of data, 1-hour resolution)
- more detailed data here?: https://services.swpc.noaa.gov/json/rtsw/

Fetch the latest real-time data:

```
./rtsw.py
```

Add the `--show` flag to see a plot of the solar wind speed.

To persist the data over time, I would load it every hour and store that fetch,
for example in a Parquet file.
This set of fetches could be combined into a single dataframe for analysis.

I wrote a script that simulates this,
so that we have data to plot for the early warning example:

```
./simulate-fetches.py
```

### Dockerization

(of the data fetching script)

Build the Docker image with:

```
docker build -t ert-afrl/fetch-noaa-rtsw:v0.1.0 .
```

We can run the Dockerized script with:

```
docker run -v /host/path/to/fetches:/app/fetches ert-afrl/fetch-noaa-rtsw:v0.1.0
```

In Kubernetes, we could reference this like so:

```yaml
spec:
  containers:
    - name: noaa-rtsw
      image: ert-afrl/fetch-noaa-rtsw:v0.1.0
      volumeMounts:
        - name: data-storage
          mountPath: /app/fetches
```

and configure a cron job to run it every hour.

### Early warning?

Using the simulated fetches, I created some plots that use the solar wind speed data to give some indication of solar storm activity.

The code is a [marimo notebook](https://marimo.io/), which can be run with

```
uvx marimo run --sandbox early-warning.py
```

(dashboard/app mode)
or edited with

```
uvx marimo edit --sandbox early-warning.py
```

(like the Python scripts, it has embedded dependency information, which `uv` will automatically resolve).

### Forecasting?

I did spend some time working with the ACE[^1] hourly solar wind archive data (`./get-training-data.py` fetches it).
I used the bulk solar variables from the `swepam` files (proton density, solar wind speed, and ion temperature) and tried to predict the _current_ solar wind based on _previous_ hours of these variables (sliding window approach, e.g. 3 hours, though I tested as much as 3 days as well).
I also tried predicting a window including the current time and future hours, e.g. 6 hours.

Maybe with additional input data from the [other ACE archive files](https://sohoftp.nascom.nasa.gov/sdb/goes/ace/monthly/), or some engineered features, I could do better.
However, it seems that solar wind speed is not very predictable.
There is little seasonality nor long-term trend.
The NASA DAGGER model (doi:[10.1029/2022SW003045](https://doi.org/10.1029/2022SW003045)), a deep-learning model, only predicts solar storm activity 30 minutes into the future (using a 2-hour time series input window).
This is a similar situation to tornado forecasting.

[^1]: This is the previous satellite, whereas the current RTSW product comes from the DSCOVR satellite measurements.
