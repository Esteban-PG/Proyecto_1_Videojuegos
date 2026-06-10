-- Detecta al jugador dentro del área y activa una variable global.
-- Props desde Tiled: trigger_id, trigger_w, trigger_h
local id        = trigger_id or "unnamed"
local half_w    = (trigger_w or 32) / 2
local half_h    = (trigger_h or 32) / 2
local activated = false

function update()
    if activated then return end
    local px, py = getPlayerPosition()
    local tx, ty = getEntityPosition(this)
    local cx = tx + half_w
    local cy = ty + half_h
    if math.abs(px - cx) < half_w and math.abs(py - cy) < half_h then
        _G["trigger_" .. id] = true
        activated = true
    end
end
