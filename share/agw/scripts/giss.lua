local io=require("io")
local ray=require("ray")
local math = require("math")
local plot=require('plot')

local tx = ray.float()
local ty = ray.float()

math.nan = 0.0/0.0

local folder = '/home/michael/Documents/ParraCAN/Data/'
local xfile = folder .. 'year.xdata'
local yfile = folder .. 'year.ydata'
local maxdate = 1
local lastdate = tonumber(0)
local firstdate = tonumber(0)
local mydate = tonumber(0)
for line in io.lines(xfile) do
	-- fixed format yyyymm,double
	local year=tonumber(line)
	tx:append(year)
end
for line in io.lines(yfile) do
	-- fixed format yyyymm,double
	local temp=tonumber(line)
	ty:append(temp)
end

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
xyplot:legend_text("Year Avg Anomaly")
xyplot:display(plotxy_template)
p:addlayer(xyplot)
p:show(true)
