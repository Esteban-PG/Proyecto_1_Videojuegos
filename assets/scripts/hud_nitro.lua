-- hud_nitro.lua
-- Updates the nitro cooldown indicator text entity every frame.
-- Reads nitro_ready / nitro_cooldown globals set by game.cpp.

function update()
    if nitro_ready then
        setTextContent(this, "NITRO: READY [SPC]")
    else
        setTextContent(this, string.format("NITRO: %.1fs", nitro_cooldown or 0))
    end
end
