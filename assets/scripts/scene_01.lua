scene = {
  sprites = {
    [0] = {assetId = "enemy_alan", filePath = "assets/images/enemy_alan.png"},
    [1] = {assetId = "player_ship", filePath = "assets/images/player_ship.png"}
  },

  fonts = {
    [0] = {assetId = "press_start_24", filePath = "assets/fonts/press_start.ttf", fontSize = 24}
  },

  keys = {
    [0] = {action = "up", key = 119},
    [1] = {action = "left", key = 97},
    [2] = {action = "down", key = 115},
    [3] = {action = "right", key = 100}
  },

  buttons = {
    [0] = {name = "mouse_left_button", button = 1},
    [1] = {name = "mouse_right_button", button = 2}
  },

  entities = {
    [0] = {
      components = {
        circle_collider = { radius = 8, width = 16, height = 16 },
        rigid_body = { velocity = {x = 0, y = 0} },
        script = { path = "./assets/scripts/player.lua" },
        sprite = { asset_id = "player_ship", width = 16, height = 16, src_rect = {x = 16, y = 0} },
        transform = { position = {x = 400.0, y = 300.0}, scale = {x = 2.0, y = 2.0}, rotation = 0.0 }
      }
    },
    [1] = {
      components = {
        circle_collider = { radius = 8, width = 16, height = 16 },
        rigid_body = { velocity = {x = 50.0, y = 0} },
        sprite = { asset_id = "enemy_alan", width = 16, height = 16, src_rect = {x = 0, y = 0} },
        animation = { num_frames = 6, frame_rate_speed = 10, is_looping = true },
        transform = { position = {x = 200.0, y = 100.0}, scale = {x = 2.0, y = 2.0}, rotation = 0.0 }
      }
    },
    [2] = {
      components = {
        circle_collider = { radius = 8, width = 16, height = 16 },
        rigid_body = { velocity = {x = -50.0, y = 0} },
        sprite = { asset_id = "enemy_alan", width = 16, height = 16, src_rect = {x = 0, y = 0} },
        animation = { num_frames = 6, frame_rate_speed = 10, is_looping = true },
        transform = { position = {x = 600.0, y = 100.0}, scale = {x = 2.0, y = 2.0}, rotation = 0.0 }
      }
    },
    [3] = {
      components = {
        clickable = {},
        text = { text = "Score: 100!",
        font_asset_id = "press_start_24",
        color = {r = 150, g = 0, b = 150, a = 255} },
        transform = { position = {x = 500.0, y = 50.0},
        scale = {x = 1.0, y = 1.0},
        rotation = 0.0 }
      }
    }
  }
}