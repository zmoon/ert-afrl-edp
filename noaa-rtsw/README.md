# NOAA real-time solar wind (RTSW) data

https://www.swpc.noaa.gov/products/real-time-solar-wind

Relevant data URLs:

- https://services.swpc.noaa.gov/products/geospace/propagated-solar-wind-1-hour.json
  (recent data, the URL shared in the problem description)
- https://services.swpc.noaa.gov/products/geospace/propagated-solar-wind.json
  (more data)
- more detailed data here?: https://services.swpc.noaa.gov/json/rtsw/

I've created a simple script to load and plot
the current real-time solar wind speed.

To persist the data over time, I would load it every hour and store that fetch,
for example in a Parquet file.
This set of fetches could be combined into a single dataframe for analysis.

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
