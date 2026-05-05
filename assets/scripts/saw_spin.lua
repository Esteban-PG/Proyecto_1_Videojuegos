-- saw_spin.lua
-- Rotates its entity continuously (spinning saw obstacle).
-- Requires that delta_time is set as a Lua global by game.cpp each frame.

local angle = 0.0
local SPIN_SPEED = 200.0   -- degrees per second

function update()
    angle = angle + SPIN_SPEED * delta_time
    if angle >= 360.0 then angle = angle - 360.0 end
    setEntityRotation(this, angle)
end
