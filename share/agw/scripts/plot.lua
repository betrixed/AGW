-- My Test Script

print("Hello!\n")
local sqlite3=require "sqlite3"
print("Lua lsqite3 version is  " .. sqlite3.lversion())
sqlite3.open(app.dbpath())


local ray=require "ray"
local s = ray.float(10)
s:set(0,1.0)
print("size ", s:size(), " value ", s:get(0))


local s = ray.year_month(1960,0,1969,11)
print("size ", s:size(), " value ", s:get(0))

local date=require "date"
d = date.new(2015,2,26)
print(d)


local math=require "math"

local x = ray.range(201,-10.0, 10.0)
local y = ray.float(201)
for i=0,200 do
    local val=x:get(i)
    y:set(i,math.sin(val))
end
y:setLabel("test10")
x:setLabel("xdata")

local plot=require "plot"
local p = plot.new();
p:addxy(x,y)
p:title("Test Plot Title")
p:xlabel("xlabel with line\nAnd Units")
p:ylabel("ylabel with linebreak\nAnd Units")
p:legend(true)
p:save("plot-1.plot")
p:exportCSV("plot-1.csv")
local p2 = plot.load("plot-1.plot")
p2:show(true)










