local io=require("io")
local ray=require("ray")
local math = require("math")
local plot=require('plot')
local app=require('app')
local stats = require('stats')

local ty = ray.float()
local tx = ray.float()

--local filename = "./plots/NewAnalysis.aravg.ann.land_ocean.90S.90N.asc"

local filename = app.fileChooser("Annual Temp. to read", "asc")

if filename==null then
	return
end

for line in io.lines(filename) do
	-- fixed format yyyymm,double
	local year=line:sub(1,4)
	local temp=line:sub(8)

	tx:append(year)
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

-- rebase the temperature anomalies to the average between 1880 and 1920
-- the data are known to be in year order
local dlen = tx:size()
local bx = ray.float()
local by = ray.float()

for i=0,dlen-1,1
do
	local year = tx:get(i)
	if (year >= 1880) and (year <= 1920) then
		bx:append(year)
		by:append(ty:get(i))
	end
end

local nstat = stats.new(by)
local baseline = nstat:avg()

for i=0,dlen-1,1
do
	ty:set(i,ty:get(i)-baseline)
end

local p = plot.new();
local xyplot = plot.plotxy(tx, ty)
xyplot:legend_text("anomaly")
xyplot:display(plotxy_template)
p:addlayer(xyplot)
p:show(true)



