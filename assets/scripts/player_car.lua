-- player_car.lua
-- Handles death → reload level and win → go to next level.
-- Actual movement (velocity, rotation) is driven by CarMovementSystem in C++.
-- This script only needs to react to terminal states.
--
-- Set these globals in the scene file BEFORE this script entity is defined:
--   current_level = "level_01"
--   next_level    = "level_02"

-- Read level transition targets set by the scene file
local my_level = current_level or "level_01"
local nxt      = next_level    or "nitro_menu"

function update()
    if isPlayerDead(this) then
        respawnPlayer()
        return
    end

    if isPlayerWon(this) then
        triggerWin()
        return
    end
end
