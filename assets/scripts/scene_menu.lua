-- scene_menu.lua  —  Nitro Rush Main Menu
-- Buttons are rendered as coloured rectangles with text labels.
-- The UISystem fires on_click() on left-click inside the button's text bounds.

scene = {
    -- ── Colour textures ─────────────────────────────────────────────────────
    color_textures = {
        [0] = { assetId = "menu_bg",    r=10,  g=10,  b=20,  a=255 },
        [1] = { assetId = "title_bar",  r=30,  g=30,  b=80,  a=255 },
        [2] = { assetId = "btn_play",   r=0,   g=150, b=0,   a=255 },
        [3] = { assetId = "btn_normal", r=60,  g=60,  b=120, a=255 },
        [4] = { assetId = "btn_quit",   r=150, g=0,   b=0,   a=255 },
    },

    -- ── Fonts ────────────────────────────────────────────────────────────────
    fonts = {
        [0] = { assetId = "font_title", filePath = "./assets/fonts/press_start.ttf", fontSize = 32 },
        [1] = { assetId = "font_hud",   filePath = "./assets/fonts/press_start.ttf", fontSize = 14 },
        [2] = { assetId = "font_btn",   filePath = "./assets/fonts/press_start.ttf", fontSize = 16 },
    },

    -- ── Input ────────────────────────────────────────────────────────────────
    keys    = {},
    buttons = {
        [0] = { name = "left_click", button = 1 },
    },

    -- ── Entities ─────────────────────────────────────────────────────────────
    entities = {
        -- Background fill
        [0] = {
            components = {
                transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="menu_bg", width=800, height=600,
                              src_rect={x=0,y=0}, is_fixed=true },
            }
        },
        -- Title bar
        [1] = {
            components = {
                transform = { position={x=0,y=55}, scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="title_bar", width=800, height=75,
                              src_rect={x=0,y=0}, is_fixed=true },
            }
        },
        -- Title text
        [2] = {
            components = {
                transform = { position={x=185,y=72}, scale={x=1,y=1}, rotation=0 },
                text = {
                    text = "NITRO RUSH",
                    font_asset_id = "font_title",
                    color = { r=255, g=220, b=0, a=255 },
                    is_fixed = true
                },
            }
        },

        -- PLAY button
        [3] = {
            components = {
                transform  = { position={x=250,y=175}, scale={x=1,y=1}, rotation=0 },
                sprite     = { asset_id="btn_play", width=300, height=44,
                               src_rect={x=0,y=0}, is_fixed=true },
                text       = {
                    text = "   PLAY (Level 1)",
                    font_asset_id = "font_btn",
                    color = { r=255, g=255, b=255, a=255 },
                    is_fixed = true
                },
                clickable  = {},
                script     = { path = "./assets/scripts/menu_play_button.lua" },
            }
        },

        -- Level 1
        [4] = {
            components = {
                transform  = { position={x=250,y=235}, scale={x=1,y=1}, rotation=0 },
                sprite     = { asset_id="btn_normal", width=300, height=40,
                               src_rect={x=0,y=0}, is_fixed=true },
                text       = {
                    text = "   Level 1 - City",
                    font_asset_id = "font_btn",
                    color = { r=200, g=255, b=200, a=255 },
                    is_fixed = true
                },
                clickable  = {},
                script     = { path = "./assets/scripts/menu_level1_button.lua" },
            }
        },

        -- Level 2
        [5] = {
            components = {
                transform  = { position={x=250,y=290}, scale={x=1,y=1}, rotation=0 },
                sprite     = { asset_id="btn_normal", width=300, height=40,
                               src_rect={x=0,y=0}, is_fixed=true },
                text       = {
                    text = "   Level 2 - Mountain",
                    font_asset_id = "font_btn",
                    color = { r=200, g=220, b=255, a=255 },
                    is_fixed = true
                },
                clickable  = {},
                script     = { path = "./assets/scripts/menu_level2_button.lua" },
            }
        },

        -- Level 3
        [6] = {
            components = {
                transform  = { position={x=250,y=345}, scale={x=1,y=1}, rotation=0 },
                sprite     = { asset_id="btn_normal", width=300, height=40,
                               src_rect={x=0,y=0}, is_fixed=true },
                text       = {
                    text = "   Level 3 - Factory",
                    font_asset_id = "font_btn",
                    color = { r=255, g=200, b=200, a=255 },
                    is_fixed = true
                },
                clickable  = {},
                script     = { path = "./assets/scripts/menu_level3_button.lua" },
            }
        },

        -- Quit
        [7] = {
            components = {
                transform  = { position={x=250,y=415}, scale={x=1,y=1}, rotation=0 },
                sprite     = { asset_id="btn_quit", width=300, height=40,
                               src_rect={x=0,y=0}, is_fixed=true },
                text       = {
                    text = "   QUIT",
                    font_asset_id = "font_btn",
                    color = { r=255, g=255, b=255, a=255 },
                    is_fixed = true
                },
                clickable  = {},
                script     = { path = "./assets/scripts/menu_quit_button.lua" },
            }
        },

        -- Controls hint
        [8] = {
            components = {
                transform = { position={x=10,y=570}, scale={x=1,y=1}, rotation=0 },
                text = {
                    text = "W/UP=Drive  A/D=Turn  SPACE=Nitro  ESC=Pause",
                    font_asset_id = "font_hud",
                    color = { r=130, g=130, b=130, a=255 },
                    is_fixed = true
                },
            }
        },
    },
}
