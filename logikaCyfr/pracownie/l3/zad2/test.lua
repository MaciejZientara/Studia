for x=0,1000 do
local a = math.random(0, 0xffff)
local b = math.random(0, 0xffff)
sim.setinput("a", a)
sim.setinput("b", b)
sim.sleep(50)
assert(tonumber(sim.getoutput("o"):tointeger()) == ((a + b) & 0xffff),
"Error: a=" .. a .. " b=" .. b .. " o=" .. tostring(sim.getoutput("o"):tointeger()))
end
print("OK!")