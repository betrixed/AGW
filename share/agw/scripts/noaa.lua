local io=require("io")
local ray=require("ray")
local math = require("math")
local plot=require('plot')

local ty = ray.float()
math.nan = 0.0/0.0

local filename = "./plots/NOAA.txt"
local maxdate = 1
local lastdate = tonumber(0)
local firstdate = tonumber(0)
local mydate = tonumber(0)
for line in io.lines(filename) do
	-- fixed format yyyymm,double
	local year=line:sub(1,4)
	local month=line:sub(5,6)
	local temp=line:sub(8)

	print(year,month,temp)

	mydate = year*12 + month-1

	if lastdate == 0 then
		firstdate = mydate
	elseif lastdate < mydate-1 then
		while lastdate < mydate-1 do
			ty:append(math.nan)
			lastdate = lastdate+1
		end
	end
	ty:append(temp);
	lastdate = mydate;
	
	if mydate > maxdate then
		maxdate = mydate 
	end

end
print(firstdate, lastdate)
local year1 = math.floor(firstdate / 12)
local year2 = math.floor(lastdate / 12)
local month1 = math.floor(firstdate % 12)
local month2 = math.floor(lastdate % 12)
print(year1, month1)
print(year2, month2)

local tx = ray.year_month(year1, month1, year2, month2)

local plotxy_template = {
			linecolor = "rgb(95, 132, 144)",
			linesize = 1,
			linestyle = "dot",
			plotcolor = "black",
			plotfill = "rgb(95, 132, 144)",
			plotshape = "circle",
			plotsize = 2,
}

local p = plot.new();
local xyplot = plot.plotxy(tx, ty)
xyplot:legend_text("anomaly")
xyplot:display(plotxy_template)
p:addlayer(xyplot)
p:show(true)

