sqlite3=require('sqlite3')
app=require('app')
ray=require('ray')
plot=require('plot')
stats=require('stats')

local plotxy_template = {
			linecolor = "rgb(95, 132, 144)",
			linesize = 1,
			linestyle = "dot",
			plotcolor = "black",
			plotfill = "rgb(95, 132, 144)",
			plotshape = "circle",
			plotsize = 2,
}
local fit_template = {
			linecolor = "red",
			linesize = 3,
			linestyle = "solid",
			plotcolor = "black",
			plotfill = "rgb(187, 73, 236)",
			plotshape ="triangle",
			plotsize = 2,
}

local db = sqlite3.open(app.dbpath())
local sql = [[
SELECT  L.latitude , A.tavg from Anomaly A, gissloc L 
where L.codeId = A.station and year = 2016 and L.latitude >= 0.0 and A.monthId = 1 
order by L.latitude
]]

local stm = db:prepare(sql)

local ytemp = ray.float()
local xlat = ray.float()
while true do
    local result = stm:step()
    if result==sqlite3.ROW then
        ytemp:append(stm:get_value(1))
        xlat:append(stm:get_value(0))
    else
        break
    end
end
print("Size = ", ytemp:size())

local p = plot.new();
local xyplot = plot.plotxy(xlat, ytemp)
xyplot:legend_text("anomaly")
xyplot:display(plotxy_template)

p:addlayer(xyplot)

p:title("Anomaly vs Latitude ")
p:xlabel("Latitude")
p:ylabel("Deg. C")
p:show(true)

local b1 = ray.float();
local b2 = ray.float();
local b3 = ray.float();
local b4 = ray.float();
local b5 = ray.float();
local b6 = ray.float();
local values = {b1,b2,b3,b4,b5,b6}
-- 0<5,  5< 13, 13<23, 23<33, 33<45,45<66, 66<12
for i=0,xlat:size()-1 do
	local lat=xlat:get(i)
	if lat >= 66.0 then
		b6:append(ytemp:get(i))
	elseif lat >= 45.0 then
		b5:append(ytemp:get(i))
	elseif lat >= 33.0 then
		b4:append(ytemp:get(i))
	elseif lat >= 13.0 then
		b3:append(ytemp:get(i))
	elseif lat >= 5.0 then
		b2:append(ytemp:get(i))
	else
		b1:append(ytemp:get(i))	
	end
end
local statn = stats.new()

for k,v in pairs(values) do
	statn:calc(v)
	print(k, statn:avg(), statn:stddev())
end
