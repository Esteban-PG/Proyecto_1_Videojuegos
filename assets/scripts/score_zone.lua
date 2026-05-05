-- score_zone.lua
-- Awards points when the player moves past (above) this entity's Y position.
--
-- Set the global `zone_points` in the scene file before the entity that uses
-- this script so each score zone captures the correct value at load time:
--
--   zone_points = 200   -- points for the NEXT score_zone entity

-- Capture the point value at load time (before zone_points might change)
local pts     = zone_points or 100
local scored  = false

function update()
    if scored then return end

    local px, py = getPlayerPosition()
    local sx, sy = getEntityPosition(this)

    -- Player has passed this zone (moved above it on screen / smaller Y)
    if py < sy - 10 then
        addScore(pts)
        scored = true
    end
end
