local app=require('app')
local ray=require('ray')
local string=require('string')
local sqlite3=require('sqlite3')
local grid=app.global("grid80")

function createBoxDetails(boxnum)
--[[
There are 4 + 8 + 12 + 16 big boxes, and 10x10 sub-boxes, in each hemisphere!
Better make sure we go through them all. It might take some time. Number 1 to 80!
init table with details
]]
	if (boxnum < 1) or (boxnum > 80) then return nil end
	local box = {}
        box["ix"] = boxnum
	if boxnum <= 40 then
		if boxnum > 24 then -- 16 big boxes
			 local rowix = 4
			 box["row"] = rowix
			 box["sign"] = 1
			 local width = 360.0/16
			 box["width"] = width
			 box["east"] = -180 + (boxnum - 25) * width
			 box["west"] = -180 + (boxnum - 24) * width
		elseif boxnum > 12 then
			 rowix = 3
			 box["row"] = rowix
			 box["sign"] = 1
			 local width = 360.0/12
			 box["width"] = width
			 box["east"] = -180 + (boxnum - 13) * width
			 box["west"] = -180 + (boxnum - 12) * width
		elseif boxnum > 4 then
			 rowix = 2
			 box["row"] = rowix
			 box["sign"] = 1
			 local width = 360.0/8
			 box["width"] = width
			 box["east"] = -180 + (boxnum - 5) * width
			 box["west"] = -180 + (boxnum - 4) * width		
		else
			 rowix = 1
			 box["row"] = rowix
			 box["sign"] = 1
			 local width = 360.0/4
			 box["width"] = width
			 box["east"] = -180 + (boxnum - 1) * width
			 box["west"] = -180 + boxnum * width		
		end
	else
		local sign = -1   -- numbering goes backwards to south pole row
		if boxnum <= 56 then
			 rowix = 4
			 box["row"] = rowix
			 box["sign"] = -1
			 local width = 360.0/16
			 box["width"] = width
			 box["east"] = -180 + (boxnum - 41) * width
			 box["west"] = -180 + (boxnum - 40) * width					
		elseif boxnum <= 68 then
			 rowix = 3
			 box["row"] = rowix
			 box["sign"] = -1
			 local width = 360.0/12
			 box["width"] = width
			 box["east"] = -180 + (boxnum - 57) * width
			 box["west"] = -180 + (boxnum - 56) * width					
		elseif boxnum <= 76 then
			 rowix = 2
			 box["row"] = rowix
			 box["sign"] = -1
			 local width = 360.0/8
			 box["width"] = width
			 box["east"] = -180 + (boxnum - 69) * width
			 box["west"] = -180 + (boxnum - 68) * width	
		else -- boxnum <= 80
			 rowix = 1
			 box["row"] = rowix
			 box["sign"] = -1
			 local width = 360.0/4
			 box["width"] = width
			 box["east"] = -180 + (boxnum - 77) * width
			 box["west"] = -180 + (boxnum - 76) * width	
		end
	end
	return box
end

local latrow = 0
local boxes = {}
local db = sqlite3.open(app.dbpath())
local result = db:execute("Select load_extension('mod_spatialite')")
if (result ~= sqlite3.OK) then print('load mod_spatialite failed') return end
for bb = 1,80 do
	local box = createBoxDetails(bb)
	boxes[#boxes + 1] = box
	local subboxes = {}
	local gridix = (box["row"]-1)*20
	local sign = box["sign"]
	for row = 0,9 do
		local ix = gridix + row*2 -- top is polewards
		local center = grid:get(ix)*sign
		local bottom = grid:get(ix+1)*sign -- bottom is equator-wards
		local top = (ix==0) and sign*90.0 or grid:get(ix-1)*sign
		local widthlg = box["width"] / 10.0
		for col = 0,9 do
			local eastlg = box["east"] + widthlg * col
			local midlog = eastlg + widthlg / 2.0
			local sql = "Select A.* from gissloc A where Distance(A.Geometry,GeomFromText('Point("
			sql = sql .. string.format("%g,%g",center,midlog) .. ")',4326)) < 1200000"
			print(sql)
			local stm = db:prepare(sql)
			while true do
			    local result = stm:step()
			    if result==sqlite3.ROW then
			        print(stm:get_value(0),stm:get_value(1))
			    else
			        break
			    end
			end

		end
	end
end




