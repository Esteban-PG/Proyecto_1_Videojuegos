-- Global variables
local player_velocity = 150.0
-- Normalize the velocity to prevent faster diagonal movement
local fixed_player_velocity = math.sqrt(player_velocity * player_velocity / 2)

function update()
  setVelocity(this, 0.0, 0.0)
  vel_x, vel_y = getVelocity(this)
  if isActionActivated("up") then
    vel_y = vel_y - 1
  end
  if isActionActivated("left") then
    vel_x = vel_x - 1
  end
  if isActionActivated("down") then
    vel_y = vel_y + 1
  end
  if isActionActivated("right") then
    vel_x = vel_x + 1
  end

  local current_velocity = player_velocity
  if vel_x ~= 0 and vel_y ~= 0 then
    current_velocity = fixed_player_velocity
  end

  vel_x = vel_x * current_velocity
  vel_y = vel_y * current_velocity

  setVelocity(this, vel_x, vel_y)
end