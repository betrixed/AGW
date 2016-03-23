local app=require('app')
local ray=require('ray')

local grid=app.global("grid40")

for i = 0, grid:size()-1 do
   print(i, grid:get(i))
end


