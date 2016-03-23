--[[ Script to generate Grid80 area boxes. 
Generate big area latitude values (L1, L2, L3)
Generate for each the sub-boxes of 10x10 equal areas.
8000 sub-boxes.
Output all 40 latitude values as a Json array 
grid40 = [  x, x,  ...    ]
]]

local math= require('math')

local R_earth = 6371 -- average km radius of earth
local epsilon = 2.0e-9 -- 1.0e-9 fails to halt

function sarea(lat1,lat2,long1, long2)
-- Lat and Long are in degrees, return area in sq. km.
	A = (math.sin(lat2*2*math.pi/360.0)-math.sin(lat1*2*math.pi/360.0))*(long2-long1)
	return math.pi*A*R_earth*R_earth/180.0
end

function adjustLat(ideal, lat, ext_N, ext_E, ext_W)
-- return  "South" latitude, of ext_N, that makes the area "Ideal"
	local adjust = 0.01
	local sign = 0
	local A1 = sarea(lat,ext_N, ext_E, ext_W)
	while (math.abs(A1 - ideal) > epsilon) do
	    if A1 > ideal then
		lat = lat + adjust
		if (sign==-1) then adjust = adjust/10.0 end
		sign=1
	    else
		lat = lat - adjust
		if (sign==1) then adjust = adjust/10.0 end
		sign = -1
	    end
	    A1 = sarea(lat, ext_N, ext_E, ext_W)
	end
	return lat
end

local L1 = 66.0
local L2 = 44.0
local L3 = 22.0

local ideal = 4 * math.pi * R_earth * R_earth / 80.0

L1 = adjustLat( ideal, L1, 90.0, -180.0, -180.0 + (360.0/4))
L2 = adjustLat( ideal, L2, L1, -180, -180 + (360.0/8))
L3 = adjustLat( ideal, L3, L2,  -180, -180 + (360.0/12))

local A1 = sarea(L1, 90, -180, -180 + (360.0/4))
local A2 = sarea(L2, L1, -180, -180 + (360.0/8))
local A3 = sarea(L3, L2, -180, -180 + (360.0/12))
local A4 = sarea(0, L3, -180, -180 + (360.0/16))

print("Refined L1 = ", L1)
print("Refined L2 = ", L2)
print("Refined L3 = ", L3)

print("A1 = ",A1)
print("A2 = ",A2)
print("A3 = ", A3)
print("A4 = ", A4)

--[[ divide into 10x10 boxes, each of (L1,90), (L2,L1), (L3,L2),(0,L3)
	Longitude is range / 10.0, so only need to do one column of latitude
--]]

-- start with lat range divided by 10.0,  with nine to approximate
local A1_range = {}
for i = 1, 9 do
	A1_range[#A1_range + 1] = 90 - ((90 - L1)/10.0)*i
end
A1_range[#A1_range + 1]  = L1

ideal = A1 / 100.0
local ext_N = 90.0
local ext_E = -180.0
local ext_W = -180.0 + (360.0/40.0)
print("ideal = ", ideal)
for i = 1, 9 do
	A1_range[i] = adjustLat(ideal, A1_range[i], ext_N, ext_E, ext_W)
	ext_N = A1_range[i]
end

ext_N = 90.0
for i = 1,10 do
	print("S = ", A1_range[i], "Area = " , sarea(A1_range[i], ext_N, ext_E, ext_W))
	ext_N = A1_range[i]
end

--[[ Same for L1-L2]]
local A2_range = {}
for i = 1, 9 do
	A2_range[#A2_range + 1] = L1 - ((L1 - L2)/10.0)*i
end
A2_range[#A2_range + 1]  = L2

ideal = A2 / 100.0
local ext_N = L1
local ext_E = -180.0
local ext_W = -180.0 + (360.0/80.0)
print("ideal = ", ideal)
for i = 1, 9 do
	A2_range[i] = adjustLat(ideal, A2_range[i], ext_N, ext_E, ext_W)
	ext_N = A2_range[i]
end

ext_N = L1
for i = 1,10 do
	print("S = ", A2_range[i], "Area = " , sarea(A2_range[i], ext_N, ext_E, ext_W))
	ext_N = A2_range[i]
end
--[[ Same for L2-L3]]
local A3_range = {}
for i = 1, 9 do
	A3_range[#A3_range + 1] = L2 - ((L2 - L3)/10.0)*i
end
A3_range[#A3_range + 1]  = L3

ideal = A3 / 100.0
local ext_N = L2
local ext_E = -180.0
local ext_W = -180.0 + (360.0/120.0)
print("ideal = ", ideal)
for i = 1, 9 do
	A3_range[i] = adjustLat(ideal, A3_range[i], ext_N, ext_E, ext_W)
	ext_N = A3_range[i]
end

ext_N = L2
for i = 1,10 do
	print("S = ", A3_range[i], "Area = " , sarea(A3_range[i], ext_N, ext_E, ext_W))
	ext_N = A3_range[i]
end
--[[ Same for L3-0]]
local A4_range = {}
for i = 1, 9 do
	A4_range[#A4_range + 1] = L3 - (L3/10.0)*i
end
A4_range[#A4_range + 1]  = 0.0

ideal = A4 / 100.0
local ext_N = L3
local ext_E = -180.0
local ext_W = -180.0 + (360.0/160.0)
print("ideal = ", ideal)
for i = 1, 9 do
	A4_range[i] = adjustLat(ideal, A4_range[i], ext_N, ext_E, ext_W)
	ext_N = A4_range[i]
end

ext_N = L3
for i = 1,10 do
	print("S = ", A4_range[i], "Area = " , sarea(A4_range[i], ext_N, ext_E, ext_W))
	ext_N = A4_range[i]
end

local io = require('io')
local fout = io.open('grid40.json','w')
fout:write('{\n\t"grid40" : [\n\t')
for i = 1,10 do fout:write(A1_range[i], ",\n\t") end
for i = 1,10 do fout:write(A2_range[i], ",\n\t") end
for i = 1,10 do fout:write(A3_range[i], ",\n\t") end
for i = 1,9 do fout:write(A4_range[i], ",\n\t") end
fout:write("0.0\n\t")
fout:write("]\n")
fout:write("}\n")
fout:close()
