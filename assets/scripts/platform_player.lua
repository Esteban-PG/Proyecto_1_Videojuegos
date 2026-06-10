-- Máquina de estados de animación + muerte/victoria.
-- Layout del sprite sheet:
--   Fila 0 (ROW_FRONT): mirando al frente
--   Fila 1 (ROW_SIDE):  mirando a la derecha — se usa para ambas
--                        direcciones horizontales; flipX voltea para la izquierda

local ROW_FRONT = 0
local ROW_SIDE  = 1

-- {assetId, numFrames, framesPerRow, fps, isLoop}
local ANIM = {
    idle  = { "player_idle",  2, 2,  6, true  },
    walk  = { "player_walk",  4, 4, 12, true  },
    death = { "player_death", 3, 3,  8, false },
}

local DEATH_DELAY = 3 / 8 + 0.3

local last_state  = ""
local last_row    = ROW_FRONT
local last_flip   = false
local death_timer = 0

local function play(name, row, flip)
    if last_state == name and last_row == row and last_flip == flip then return end
    last_state = name
    last_row   = row
    last_flip  = flip
    local a = ANIM[name]
    playAnimation(this, a[1], a[2], a[3], a[4], a[5], row)
    setFlipX(this, flip)
end

function update()
    -- ── Muerte ───────────────────────────────────────────────────────────
    if isPlayerDead(this) then
        play("death", ROW_FRONT, false)
        death_timer = death_timer + delta_time
        if death_timer >= DEATH_DELAY then
            death_timer = 0
            last_state  = ""
            respawnPlayer()
        end
        return
    end
    death_timer = 0

    -- ── Victoria ─────────────────────────────────────────────────────────
    if isPlayerWon(this) then
        triggerWin()
        return
    end

    -- ── Movimiento ───────────────────────────────────────────────────────
    local vx, _ = getVelocity(this)
    if vx > 10 then
        play("walk", ROW_SIDE, false)       -- derecha: sin flip
    elseif vx < -10 then
        play("walk", ROW_SIDE, true)        -- izquierda: flip horizontal
    else
        play("idle", last_row, last_flip)   -- idle: mantiene última dirección
    end
end
