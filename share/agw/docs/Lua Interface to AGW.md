Lua Interface to AGW

The Lua interface provides a small number of lib modules.
The lua language is a small embedded interpreter, highly refined, allowing the C++ code and objects to be accessed.
New object methods were added on an as-needed basis , for small tasks like data-import from text files in ad-hoc formats.


Ray
====

local ray=require("ray")

A "ray" is the module used for access to Series (or Array-like) objects

?size is optional initial size argument

-- float(?size) - return a new float array object, with 0, or size elements.

local ty = ray.float(?size)

-- double(?size) - return a new double array object

local ty = ray.double(?size)

-- range() - return a "LineSeries" object. Internally this is a start and end value , and a number of points >= 2, that exist virtually between them.

-- year_month() -- Specifies a date range, from a starting year,month to a end year,month.  2 points, plus the additional number of points as months between them.

-- read(filename) 
-- return an array object that was saved by a <ray_object>:save(filename)

Series
======
-- set and get various properties. For range objects "set" has limited applicability. Array values start at 0 and goto size() - 1

-- get the array value at index as a double 
-- seriesObject:get(index)
local value = array:get(ix)

-- set the array value at index as a double argument
-- <seriesObject:set(index, value)

local array:set(ix,value)

-- get the length ()
-- seriesObject:size()
local arrayLength = array:size()

-- array:setLabel(string)  set or get a label ("name") property of the array
-- string array:getLabel()

-- array:save(string)  
-- save as a JSON formatted text filename

-- array:append(value)   
-- extends the array length by one new value.

-- array:toJson()  returns a JSON formatted string that describes the array

To create a new series from a file created by array:save(filepath), 
used 
local x = ray.read(filepath)  


SeriesDate
==========
This wraps a boost::gregorian::date object, for storing dates as day, month, year.  It can convert to Julian, and modified Julian numbers, to make a day number, useful for a series.


NormalStat
==========
This object can calculate a number standard normal distribution statistics on a series. Its calc(<seriesObject>) method stores the results, which then can be obtained by calling its property gets.

local stats = require("stats")

local ndist = stats.new()
ndist:calc(<seriesObject>)
local asize = ndist:size()
local amin = ndist:min()
local amax = ndist:max()
local avg = ndist:avg()
local stddev = ndist:stddev()


Plot
====
local plot=require('plot')
-- plot series sx, sy
local p = plot.new();
local xyplot = plot.plotxy(sx, sy)
xyplot:legend_text("anomaly")

-- set various display properties using a lua table as a "template".
local plotxy_template = {
			linecolor = "rgb(95, 132, 144)",
			linesize = 1,
			linestyle = "dot",
			plotcolor = "black",
			plotfill = "rgb(95, 132, 144)",
			plotshape = "circle",
			plotsize = 2,
}

xyplot:display(plotxy_template)
p:addlayer(xyplot)
p:show(true)

AppData
=======
This is used get a few exported global values from the host application.
local app=require('app')

-- return a global series object called "grid80".

local grid=app.global("grid80")	
local dbfilename = app.dbpath()

Its also useful for getting a file path using the file chooser dialog.

local filepath = fileChooser("File to read","ext")   -- leave out the dot for extension.
if (filepath == null) then return end

Sqlite3
=======
There is also a loaded sqlite3 module available, 
using lsqlite3 - a SQLite database binding for Lua 5. 

The app.dbpath() property returns the file sqlite3 file name.
This allows independent connections to the application database tables.
For instance 'mod_spatialite' is available for database tables.

local sqlite3=require('sqlite3')
local db = sqlite3.open(app.dbpath())
local result = db:execute("Select load_extension('mod_spatialite')")
if (result ~= sqlite3.OK) then print('load mod_spatialite failed') return end
