AGW : Anthropogenic Global Warming ,  or Application Graph Widgets
==================================================================

The application started out as a wxWidgets application, which processed some local weather temperature and rainfall data, and did simple time period average comparisons.  I did this for an online course - Coursera "Climate Change Conversations".  I finished and do recommend that course.

Now the ambition is to make the internal data series, graphs and spatialite database objects accessible to the embedded Lua scripting language, and be able to process global historical climate data.  This requires some study of published methods of analysing such online data, in order produce analysis, output data, graphs and maps that are accurate representations of the data, show trends or lack of them clearly, and have reasonable portable file formats.

Development and testing is being done on a Linux Platform, and Windows OS.

Mac OS port will be done when there is enough functionality to make it worthwhile, and I don't give up on the whole idea before then.

A number of open source libraries and code are used, which make this possible.

Lua, Sqlite3,  Spatialite, JsonCpp

Especial gratitude for the spatialite project documentation at http://gaia-gis.it/


Current Immediate Goal :
------------------------
	Implement the global temperature analysis method described clearly in
		Hansen and Lebedeff 1987 : Global Trends of Measured Surface Temperature (GTMST).

    Graph results, ups and downs, of yearly temperature anomaly. look similar to 2010 paper - Global Surface Temperature Change - Hansen - Ruedy Sato and Lo,
        but not precisely.

Source code resources to compile on debian 11.

Currently uses wxWidgets-3.1.5.  Built as static libraries, unicode enabled.
Build scripts call wx-config.

Lua source code 5.4.4 from lua.org,  from AGW/src symbol link:  ln -s lua-5.4.4/src lua

apt install libspatialite-dev

apt install shapelib libshp-dev

apt install libjsoncpp-dev

apt install libsqlite3-mod-spatialite

apt install spatialite-bin

apt install libboost-dev

### Data sources:

Using ghcn version 4 formats. Initial version of AGW used ghcn version 3.

General information:
https://www.ncei.noaa.gov/access/metadata/landing-page/bin/iso?id=gov.noaa.ncdc:C00950

Data links
https://www1.ncdc.noaa.gov/pub/data/ghcn/v4/

NOAA provides 3 similar data sets for ghcn version 4. 
All are for average temperature.

Only one set at a time can be used by the AGW application and a database.
https://www1.ncdc.noaa.gov/pub/data/ghcn/v4/readme.txt

QCU -- quality control, unadjusted values

QCF -- quality control, adjusted using pairwise homogeneity algorithm.

QFE -- same as QCF. Readme.txt says only years 1961 - 2010 are provided. 
       but the archive file size is about the same size as QCF and QCU (42 MB)


For example, download the ghcnm.tavg.latest.qcu.tar.gz

Unpack  -- tar -xf ghcnm.tavg.latest.qcu.tar.gz

Launch the AGW application

Import of station data should show in the log view (Window - Log menu)

Import station data ( \*.qcu.inv ) 
Import temperature data ( \*.qcu.dat)

After new database create a successful imports log should have something like this.

14:39:53: First time initialize for spatialite/sqlite3, please wait . . .
14:39:53: Thread Entry
14:40:00: spatialite init completed
14:40:00: spatialite data open
14:40:00: dataDir /home/michael/dev/AGW/share/agw , mapdir maps
14:40:00: country table with 252 rows
14:40:00: Opened /home/michael/.local/share/AGW/climate.sqlite3
14:40:00: GISS Tables Created
14:40:00: Station Sets installed
14:40:47: Updated 27728 records
14:42:07: Selected /home/michael/Downloads/ghcnm.v4.0.1.20220402/ghcnm.tavg.v4.0.1.20220402.qcu.dat
14:44:09: maxLine length = 115
14:44:09: Updated 1443058 records


### Make station sets

AGW enables sets of stations to be defined by a query and saved.

Initially there aren't any sets. Go to Window - Station subsets.

Select the "Query" tab, enter "All stations", Click on the "+" button, and then 
the Update button in the stations sets list on the left.

"All Stations" query means no restriction, and selection should display the full list
of station data, currently 27,728 items as of April 2022. They are alphabetically sorted.

### Make a sub-set

Suppose we take the area with a 100 km radius around Bourke NSW.

Look up BOURKE_POST_OFFICE in the All stations list, and get its latitude, longitude.

In the query options, tick "Circle", enter radius 100, Long 145.936, Lat -30.0917

Give the set a new name, eg "Bourke region".  Click on Update button.

### Get a TAVG trend

To see how a region or entire global temperature is trending over decades, a method of crunching the data to just one value for each year is required. This means condensing the seasonal and regional variations to comparable index values, then averaging them.

On the Trend tab, the baseline data year range is used for a monthly average data anomaly. 
This allows stations to act as their own month - anomaly reference, and allows comparison and averaging of anomaly values across stations and regions. Of course it is important that the stations have enough data values in anomaly baseline range of years. The anomalies are calculated on each month so that they can be averaged between different months as well as regions.

With for the 3 stations in Bourke region with linear regression over the years 1970 to 2021, Anomaly baseline from 1980 to 2021, gives an anomaly graph that shows a 50 year warming trend from 1970 to 2021 of 0.11 degrees per decade, or 0.55 degrees since 1970. The R value is only 0.27, so this indicates temperature anomaly data is "noisey". The variations in anomaly are much bigger than the "signal" here. Also the Bourke Post Office records go back over 100 years to start in 1870's. The data in the far past is also noisey and less likely to be measured and recorded to same accuracy used by standard instruments and techniques of more recent times.

### Include some more data for averaging

What is the average warming for weather stations at around 30 deg south in Australia? Create a band query for stations.  Latitude -32 to -28 , Country name is Australia (use drop down button).  

This gives around 200 stations. Averaging the anomaly has reduced the "noise" somewhat, and increased the R-val to 0.56.   The averaged 50 year warming increase is 0.75 degrees.


### Use all the stations

The same trend / anomaly over all 27,000 plus stations gives a overall 50 year warming as 1.05 degrees.
Still some noise with an R-val of 0.77 for the regression over the last 50 years.


### Weighted land area coverage.

James Hansen & helpers published a reasonable method of weighting temperature anomaly by land area, to get an area averaged warming. This algorithm is implemented in AGW and produces similar anomaly trend results. Stations as far apart as 1200 kilometres have correlated temperature records, corresponding to the often large spatial extent of our weather system patterns. All the stations are used, but their degree of closeness and density is adjusted by dividing the global into many equal areas. This is activated from the Plot - Global Temperature menu.

The plot data is automatically saved as "globaltemp_anom.plot". 

Now values from highly populated regions with irregularly scattered and many closely located weather stations, is put on a par with regions of sparse population and data from fewer stations, such as polar regions or deserts.

This is very important, because the largest global heating increases are happening in polar regions, but these regions have the smallest number of surface area temperature recording stations. By specifying area weighting for temperature anomaly, the bias of representation count is reduced.

The 50 year warming increase, on global land surface temperature stations weighted by area, is now something like 1.4 degrees.  The R-val is 0.93. The trend is still rising. At the implied average rate of 0.28 degrees C per decade, the 2015 "aspirational" target of no more than 1.5 degrees of global heating has already happened by 2030, for which current mitigation action is already too late.

Of course you can say that most of this warming will be in Polar regions, and it might not affect industrial civilisation very much. We don't live at the poles. Time and rising polar heat brings forwards the acceleration of Greenland and Antarctic ice sheet melt and collapse. Global sea level rise threatens a lot of ocean fronted human real estate. There is going to be a big decline  in global agricultural grain production. There is a large global area of heat vulnerable tropical regions already near limits of maximum heat tolerance. Large regions of human populations will find themselves forced to move.

Progress
********


2016/03/29 - Multi-threading of GTMST, putting each large Area into a job queue.
2016/03/30 - HL 1987 algorithm - Fixed bug where number of threads changes result weightings
            - Station sort ordering was reversed, database query row order fixed.

2019/03/27 -- Cloned archive to check compilability and latest climate data, on Linux



