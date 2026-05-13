-- saw_patrol.lua
-- Mueve la sierra de un lado a otro alrededor de su posición de origen.
-- Propiedades configurables desde Tiled:
--   patrol_axis  : "x" o "y"   (default "y")
--   patrol_range : píxeles      (default 80)
--   patrol_speed : px/segundo   (default 60)

local axis      = patrol_axis  or "y"
local range     = patrol_range or 80
local speed     = patrol_speed or 60
local spin      = patrol_spin  or 200   -- grados por segundo
local origin    = nil
local dir       = 1
local angle     = 0.0

function update()
    local x, y = getEntityPosition(this)

    if origin == nil then
        origin = (axis == "x") and x or y
    end

    -- Patrulla
    if axis == "x" then
        x = x + speed * dir * delta_time
        if x > origin + range then dir = -1 end
        if x < origin - range then dir =  1 end
    else
        y = y + speed * dir * delta_time
        if y > origin + range then dir = -1 end
        if y < origin - range then dir =  1 end
    end

    setEntityPosition(this, x, y)

    -- Rotación continua
    angle = angle + spin * delta_time
    if angle >= 360.0 then angle = angle - 360.0 end
    setEntityRotation(this, angle)
end
