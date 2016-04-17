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



Progress
********

2016/03/29 - Multi-threading of GTMST, putting each large Area into a job queue.
2016/03/30 - HL 1987 algorithm - Fixed bug where number of threads changes result weightings
            - Station sort ordering was reversed, database query row order fixed.


