-- hud_score.lua
-- Updates the score display text entity every frame.
-- Reads the `current_score` global set by game.cpp.

function update()
    setTextContent(this, "SCORE: " .. (current_score or 0))
end
