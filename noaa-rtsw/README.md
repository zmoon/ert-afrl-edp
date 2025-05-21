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
