for x=0,99 do
local a = math.random(0, 99)
local b = math.random(0, 99)
sim.setinput("a", '16h' .. tostring(a))
sim.setinput("b", '16h' .. tostring(b))
sim.setinput("sub", false)
sim.sleep(100)
assert(tonumber(sim.getoutput("o"):tohex()) == (a + b) % 100,
"Error: a=" .. a .. " b=" .. b)
sim.setinput("sub", true)
sim.sleep(100)
assert(tonumber(sim.getoutput("o"):tohex()) == (a - b) % 100,
"Error: a=" .. a .. " b=" .. b)
end
print("OK!")