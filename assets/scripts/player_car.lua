-- player_car.lua
-- Handles death → reload level and win → go to next level.
-- Actual movement (velocity, rotation) is driven by CarMovementSystem in C++.
-- This script only needs to react to terminal states.
--
-- Set these globals in the scene file BEFORE this script entity is defined:
--   current_level = "level_01"
--   next_level    = "level_02"

local nxt         = next_level or "nitro_menu"
local death_timer = 0
local DEATH_DELAY = 0.6   -- segundos de pausa antes del respawn

function update()
    if isPlayerDead(this) then
        death_timer = death_timer + delta_time
        if death_timer >= DEATH_DELAY then
            death_timer = 0
            respawnPlayer()
        end
        return
    end

    death_timer = 0

    if isPlayerWon(this) then
        triggerWin()
        return
    end
end
