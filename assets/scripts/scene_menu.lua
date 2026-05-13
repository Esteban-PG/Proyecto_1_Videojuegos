-- scene_menu.lua  —  Nitro Rush Main Menu

scene = {
    -- ── Colour textures ─────────────────────────────────────────────────────
    color_textures = {
        [0] = { assetId = "menu_bg",   r=8,   g=8,   b=16,  a=255 },
        [1] = { assetId = "title_bar", r=20,  g=20,  b=50,  a=255 },
        [2] = { assetId = "btn_lvl",   r=28,  g=28,  b=65,  a=255 },
        [3] = { assetId = "btn_quit",  r=65,  g=15,  b=15,  a=255 },
        [4] = { assetId = "divider",   r=255, g=200, b=0,   a=60  },
    },

    -- ── Fonts ────────────────────────────────────────────────────────────────
    fonts = {
        [0] = { assetId = "font_title",    filePath = "./assets/fonts/press_start.ttf", fontSize = 36 },
        [1] = { assetId = "font_subtitle", filePath = "./assets/fonts/press_start.ttf", fontSize = 11 },
        [2] = { assetId = "font_btn",      filePath = "./assets/fonts/press_start.ttf", fontSize = 16 },
    },

    -- ── Input ────────────────────────────────────────────────────────────────
    keys    = {},
    buttons = {
        [0] = { name = "left_click", button = 1 },
    },

    -- ── Entities ─────────────────────────────────────────────────────────────
    entities = {
        -- Fondo
        [0] = { components = {
            transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="menu_bg", width=800, height=600, src_rect={x=0,y=0}, is_fixed=true },
        }},

        -- Barra superior del título
        [1] = { components = {
            transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="title_bar", width=800, height=130, src_rect={x=0,y=0}, is_fixed=true },
        }},

        -- Línea dorada decorativa bajo el título
        [2] = { components = {
            transform = { position={x=0,y=130}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="divider", width=800, height=2, src_rect={x=0,y=0}, is_fixed=true },
        }},

        -- Título
        [3] = { components = {
            transform = { position={x=148,y=42}, scale={x=1,y=1}, rotation=0 },
            text = { text="NITRO RUSH", font_asset_id="font_title",
                     color={r=255,g=210,b=0,a=255}, is_fixed=true },
        }},

        -- Subtítulo
        [4] = { components = {
            transform = { position={x=290,y=100}, scale={x=1,y=1}, rotation=0 },
            text = { text="SELECT LEVEL", font_asset_id="font_subtitle",
                     color={r=160,g=160,b=200,a=255}, is_fixed=true },
        }},

        -- LVL 01
        [5] = { components = {
            transform = { position={x=270,y=200}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="btn_lvl", width=260, height=50, src_rect={x=0,y=0}, is_fixed=true },
            text      = { text="    LVL 01", font_asset_id="font_btn",
                          color={r=255,g=255,b=255,a=255}, is_fixed=true },
            clickable = {},
            script    = { path="./assets/scripts/menu_level1_button.lua" },
        }},

        -- LVL 02
        [6] = { components = {
            transform = { position={x=270,y=270}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="btn_lvl", width=260, height=50, src_rect={x=0,y=0}, is_fixed=true },
            text      = { text="    LVL 02", font_asset_id="font_btn",
                          color={r=255,g=255,b=255,a=255}, is_fixed=true },
            clickable = {},
            script    = { path="./assets/scripts/menu_level2_button.lua" },
        }},

        -- LVL 03
        [7] = { components = {
            transform = { position={x=270,y=340}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="btn_lvl", width=260, height=50, src_rect={x=0,y=0}, is_fixed=true },
            text      = { text="    LVL 03", font_asset_id="font_btn",
                          color={r=255,g=255,b=255,a=255}, is_fixed=true },
            clickable = {},
            script    = { path="./assets/scripts/menu_level3_button.lua" },
        }},

        -- QUIT
        [8] = { components = {
            transform = { position={x=270,y=430}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="btn_quit", width=260, height=50, src_rect={x=0,y=0}, is_fixed=true },
            text      = { text="    QUIT", font_asset_id="font_btn",
                          color={r=255,g=180,b=180,a=255}, is_fixed=true },
            clickable = {},
            script    = { path="./assets/scripts/menu_quit_button.lua" },
        }},

        -- Controles
        [9] = { components = {
            transform = { position={x=140,y=560}, scale={x=1,y=1}, rotation=0 },
            text = { text="W  ACELERAR    A/D  GIRAR    SPC  NITRO",
                     font_asset_id="font_subtitle",
                     color={r=80,g=80,b=110,a=255}, is_fixed=true },
        }},
    },
}
