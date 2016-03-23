sqlite3=require('sqlite3')
app=require('app')
ray=require('ray')
plot=require('plot')

local values = {}
values["byearfrom"] = 1950
values["byearto"] = 1980
values["tempm"] = "TMIN"
values["bset"] = "Arctic2"


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

print(sqlite3.lversion())
print(app.dbpath())
local db = sqlite3.open(app.dbpath())
local sql = [[
SELECT AVG(T.value - A.base) as anomoly, Y.year as year
  FROM gissyear Y, gisstemp T,
    ( SELECT B.locId, R.monthId, AVG(R.Value) as base
        FROM gisstemp R, gissyear B
        WHERE B.YEAR >= :byearfrom and B.YEAR <= :byearto
        and R.codeId = B.codeId and B.measure = :tempm
        and B.locID in ( Select codeId from memberstation WHERE setName = :bset )
        group by B.locId, R.monthId
    ) A
    WHERE Y.locId = A.locID and A.monthId = T.monthID
    and Y.measure = :tempm
    and Y.codeId = T.codeId
     GROUP by Y.year ORDER BY year
]]


local stm = db:prepare(sql)
stm:bind_names(values)

local ytemp = ray.float()
local xyear = ray.float()
while true do
    local result = stm:step()
    if result==sqlite3.ROW then
        ytemp:append(stm:get_value(0))
        xyear:append(stm:get_value(1))
    else
        break
    end
end
print("Size = ", ytemp:size())

local p = plot.new();
local xyplot = plot.plotxy(xyear, ytemp)
xyplot:legend_text("anomaly")
xyplot:display(plotxy_template)

p:addlayer(xyplot)

p:title("Temperature Anomaly Trend for " .. values["bset"])
p:xlabel("Year")
p:ylabel("Deg. C")

p:show(true)
-- do a regression from 1980 onwards, for current warming trend
local rx = ray.float()
local ry = ray.float()
for x = 0,xyear:size()-1 do
    local year = xyear:get(x)
    if (year >= 1980) then
        rx:append(year)
        ry:append(ytemp:get(x))
    end
end
print("regression points = ", rx:size())
local fit = plot.linefit(rx,ry)
p:addlayer(fit)
fit:legend_text(string.format("Warming %5.2g / decade,  Rval %5.2g", fit:slope()*10.0, fit:rval()))
fit:display(fit_template)
p:legend(true)
db:close()

