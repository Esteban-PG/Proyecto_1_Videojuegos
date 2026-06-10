scene = {
    color_textures = {
        [0] = { assetId="menu_bg",    r=12,  g=12,  b=22,  a=255 },
        [1] = { assetId="title_bar",  r=20,  g=20,  b=50,  a=255 },
        [2] = { assetId="btn_play",   r=30,  g=100, b=30,  a=255 },
        [3] = { assetId="btn_quit",   r=80,  g=20,  b=20,  a=255 },
        [4] = { assetId="divider",    r=255, g=200, b=0,   a=80  },
    },

    fonts = {
        [0] = { assetId="font_title",    filePath="./assets/fonts/press_start.ttf", fontSize=32 },
        [1] = { assetId="font_subtitle", filePath="./assets/fonts/press_start.ttf", fontSize=11 },
        [2] = { assetId="font_btn",      filePath="./assets/fonts/press_start.ttf", fontSize=18 },
    },

    keys    = {},
    music   = "music_menu",
    buttons = {
        [0] = { name="left_click", button=1 },
    },

    entities = {
        -- Fondo
        [0] = { components = {
            transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="menu_bg", width=800, height=600, src_rect={x=0,y=0}, is_fixed=true },
        }},

        -- Barra del título
        [1] = { components = {
            transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="title_bar", width=800, height=140, src_rect={x=0,y=0}, is_fixed=true },
        }},

        -- Línea dorada bajo el título
        [2] = { components = {
            transform = { position={x=0,y=140}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="divider", width=800, height=3, src_rect={x=0,y=0}, is_fixed=true },
        }},

        -- Título
        [3] = { components = {
            transform = { position={x=134,y=48}, scale={x=1,y=1}, rotation=0 },
            text = { text="TRAP PLATFORMER", font_asset_id="font_title",
                     color={r=255,g=210,b=0,a=255}, is_fixed=true },
        }},

        -- Subtítulo
        [4] = { components = {
            transform = { position={x=268,y=106}, scale={x=1,y=1}, rotation=0 },
            text = { text="DON'T GET TRAPPED", font_asset_id="font_subtitle",
                     color={r=140,g=140,b=180,a=255}, is_fixed=true },
        }},

        -- Botón JUGAR
        [5] = { components = {
            transform = { position={x=270,y=240}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="btn_play", width=260, height=60, src_rect={x=0,y=0}, is_fixed=true },
            text      = { text="  JUGAR", font_asset_id="font_btn",
                          color={r=255,g=255,b=255,a=255}, is_fixed=true },
            clickable = {},
            script    = { path="./assets/scripts/menu_play_button.lua" },
        }},

        -- Botón SALIR
        [6] = { components = {
            transform = { position={x=270,y=330}, scale={x=1,y=1}, rotation=0 },
            sprite    = { asset_id="btn_quit", width=260, height=60, src_rect={x=0,y=0}, is_fixed=true },
            text      = { text="  SALIR", font_asset_id="font_btn",
                          color={r=255,g=180,b=180,a=255}, is_fixed=true },
            clickable = {},
            script    = { path="./assets/scripts/menu_quit_button.lua" },
        }},

        -- Controles
        [7] = { components = {
            transform = { position={x=160,y=555}, scale={x=1,y=1}, rotation=0 },
            text = { text="A/D  MOVER     SPACE  SALTAR",
                     font_asset_id="font_subtitle",
                     color={r=70,g=70,b=100,a=255}, is_fixed=true },
        }},
    },
}
