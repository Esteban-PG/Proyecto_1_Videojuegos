-- Se mueve en una dirección cuando se activa su trigger_id.
-- Props desde Tiled: trigger_id, patrol_axis, patrol_speed, patrol_range
local id     = trigger_id   or "unnamed"
local axis   = patrol_axis  or "y"
local speed  = patrol_speed or 150
local range  = patrol_range or 300
local origin = nil
local dist   = 0
local moving = false

function update()
    if not moving then
        if _G["trigger_" .. id] == true then
            moving = true
        end
        return
    end

    local x, y = getEntityPosition(this)

    if origin == nil then
        origin = { x = x, y = y }
    end

    if axis == "y" then
        y = y + speed * delta_time
        dist = y - origin.y
    else
        x = x + speed * delta_time
        dist = x - origin.x
    end

    setEntityPosition(this, x, y)

    -- Detener al llegar al rango máximo (queda fija)
    if dist >= range then
        if axis == "y" then
            setEntityPosition(this, x, origin.y + range)
        else
            setEntityPosition(this, origin.x + range, y)
        end
        moving = false
    end
end
