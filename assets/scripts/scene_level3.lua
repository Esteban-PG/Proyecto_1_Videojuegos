-- scene_level3.lua  —  Nitro Rush: Level 3 — Factory / Night
-- Zigzag agresivo: el camino alterna izquierda-derecha en giros cerrados.
-- SEG_H pequeño para curvas definidas; overlay oscuro para sensación de noche.

current_level = "level_03"
next_level    = "nitro_menu"
zone_points   = 100

local ROAD_W = 250
local WALL_W = 120
local SEG_H  = 60    -- segmentos pequeños = giros más definidos

-- Zigzag: derecha → izquierda → derecha → izquierda → final recto
local WP = {
    {y=8200, cx=400},
    {y=7400, cx=400},  -- recto inicial
    {y=6600, cx=510},  -- giro derecha
    {y=5600, cx=270},  -- giro izquierda
    {y=4600, cx=510},  -- giro derecha
    {y=3600, cx=270},  -- giro izquierda
    {y=2600, cx=400},  -- vuelve al centro
    {y=1400, cx=400},  -- recto final
    {y=0,    cx=400},
}

local function getCX(y)
    if y >= WP[1].y  then return WP[1].cx  end
    if y <= WP[#WP].y then return WP[#WP].cx end
    for i = 1, #WP - 1 do
        local a, b = WP[i], WP[i+1]
        if y <= a.y and y >= b.y then
            local t = (a.y - y) / (a.y - b.y)
            return a.cx + t * (b.cx - a.cx)
        end
    end
    return 400
end

local E   = {}
local idx = 0

local function seg(cx, y)
    local rx = math.floor(cx - ROAD_W / 2)
    E[idx] = { components = {
        transform = { position={x=rx,        y=y}, scale={x=1,y=1}, rotation=0 },
        sprite    = { asset_id="road_factory", width=ROAD_W, height=SEG_H+2, src_rect={x=0,y=0} },
    }} ; idx = idx + 1
    E[idx] = { components = {
        transform    = { position={x=rx-WALL_W, y=y}, scale={x=1,y=1}, rotation=0 },
        sprite       = { asset_id="wall_metal", width=WALL_W, height=SEG_H+2, src_rect={x=0,y=0} },
        box_collider = { width=WALL_W, height=SEG_H+2 },
        tag          = { tag="wall" },
    }} ; idx = idx + 1
    E[idx] = { components = {
        transform    = { position={x=rx+ROAD_W, y=y}, scale={x=1,y=1}, rotation=0 },
        sprite       = { asset_id="wall_metal", width=WALL_W, height=SEG_H+2, src_rect={x=0,y=0} },
        box_collider = { width=WALL_W, height=SEG_H+2 },
        tag          = { tag="wall" },
    }} ; idx = idx + 1
end

local function sawCentral(cx, y, w, h, r, spin)
    local c = {
        transform      = { position={x=math.floor(cx-w/2), y=y}, scale={x=1,y=1}, rotation=0 },
        sprite         = { asset_id="saw_red", width=w, height=h, src_rect={x=0,y=0} },
        circle_collider= { radius=r, width=w, height=h },
        tag            = { tag="saw" },
    }
    if spin then c.script = { path="./assets/scripts/saw_spin.lua" } end
    E[idx] = { components=c } ; idx = idx + 1
end

local function sawOrange(cx, y, w, h, r)
    E[idx] = { components = {
        transform      = { position={x=math.floor(cx-w/2), y=y}, scale={x=1,y=1}, rotation=0 },
        sprite         = { asset_id="saw_orange", width=w, height=h, src_rect={x=0,y=0} },
        circle_collider= { radius=r, width=w, height=h },
        tag            = { tag="saw" },
        script         = { path="./assets/scripts/saw_spin.lua" },
    }} ; idx = idx + 1
end

local function sawWall(cx, y, side)
    local w, h, r = 80, 40, 20
    local x = side == "left" and math.floor(cx - ROAD_W/2)
                              or  math.floor(cx + ROAD_W/2 - w)
    E[idx] = { components = {
        transform      = { position={x=x, y=y}, scale={x=1,y=1}, rotation=0 },
        sprite         = { asset_id="saw_red", width=w, height=h, src_rect={x=0,y=0} },
        circle_collider= { radius=r, width=w, height=h },
        tag            = { tag="saw" },
    }} ; idx = idx + 1
end

local function sawDouble(cx, y)
    local w, h, r = 46, 46, 23
    E[idx] = { components = {
        transform      = { position={x=math.floor(cx-ROAD_W/2+4), y=y}, scale={x=1,y=1}, rotation=0 },
        sprite         = { asset_id="saw_red", width=w, height=h, src_rect={x=0,y=0} },
        circle_collider= { radius=r, width=w, height=h },
        tag            = { tag="saw" },
    }} ; idx = idx + 1
    E[idx] = { components = {
        transform      = { position={x=math.floor(cx+ROAD_W/2-w-4), y=y}, scale={x=1,y=1}, rotation=0 },
        sprite         = { asset_id="saw_red", width=w, height=h, src_rect={x=0,y=0} },
        circle_collider= { radius=r, width=w, height=h },
        tag            = { tag="saw" },
    }} ; idx = idx + 1
end

local function ramp(cx, y)
    E[idx] = { components = {
        transform    = { position={x=math.floor(cx-ROAD_W/2), y=y}, scale={x=1,y=1}, rotation=0 },
        sprite       = { asset_id="ramp_yellow", width=ROAD_W, height=50, src_rect={x=0,y=0} },
        box_collider = { width=ROAD_W, height=50 },
        tag          = { tag="ramp" },
    }} ; idx = idx + 1
end

local function gap(cx, y, h)
    h = h or 190
    E[idx] = { components = {
        transform    = { position={x=math.floor(cx-ROAD_W/2), y=y}, scale={x=1,y=1}, rotation=0 },
        sprite       = { asset_id="gap_void", width=ROAD_W, height=h, src_rect={x=0,y=0} },
        box_collider = { width=ROAD_W, height=h },
        tag          = { tag="gap" },
    }} ; idx = idx + 1
end

local function scoreZone(cx, y)
    E[idx] = { components = {
        transform    = { position={x=math.floor(cx-ROAD_W/2), y=y}, scale={x=1,y=1}, rotation=0 },
        box_collider = { width=ROAD_W, height=20 },
        tag          = { tag="score_zone" },
        script       = { path="./assets/scripts/score_zone.lua" },
    }} ; idx = idx + 1
end

local function checkpoint(cx, y)
    E[idx] = { components = {
        transform    = { position={x=math.floor(cx-ROAD_W/2), y=y}, scale={x=1,y=1}, rotation=0 },
        sprite       = { asset_id="checkpoint", width=ROAD_W, height=40, src_rect={x=0,y=0} },
        box_collider = { width=ROAD_W, height=40 },
        tag          = { tag="checkpoint" },
    }} ; idx = idx + 1
end

-- ── GENERAR CAMINO ────────────────────────────────────────────────────────────
do local y = 8200
   while y >= -SEG_H do
       seg(getCX(y), y)
       y = y - SEG_H
   end
end

-- Overlay oscuro tipo noche (se renderiza después del camino, antes del jugador)
E[idx] = { components = {
    transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
    sprite    = { asset_id="dark_overlay", width=800, height=600,
                  src_rect={x=0,y=0}, is_fixed=true },
}} ; idx = idx + 1

-- ── OBSTÁCULOS ────────────────────────────────────────────────────────────────

-- Recto inicial
sawWall(getCX(7600), 7600, "left")
sawWall(getCX(7600), 7600, "right")
scoreZone(getCX(7450), 7450)

-- Giro derecha (cx≈510)
sawOrange(getCX(7000), 7000, 65, 65, 32)
scoreZone(getCX(6850), 6850)

ramp(getCX(6600), 6600)
gap(getCX(6120), 6120, 190)

sawDouble(getCX(5800), 5800)
scoreZone(getCX(5650), 5650)

-- Giro izquierda (cx≈270)
sawCentral(getCX(5400), 5400, 55, 55, 28, false)

ramp(getCX(5000), 5000)
gap(getCX(4520), 4520, 190)

sawOrange(getCX(4300), 4300, 70, 70, 35)
scoreZone(getCX(4150), 4150)

-- Giro derecha (cx≈510)
sawDouble(getCX(3800), 3800)
scoreZone(getCX(3650), 3650)

ramp(getCX(3600), 3600)
gap(getCX(3120), 3120, 190)

-- Giro izquierda (cx≈270)
sawCentral(getCX(2800), 2800, 60, 60, 30, true)
scoreZone(getCX(2650), 2650)

-- Tramo central de vuelta
ramp(getCX(2400), 2400)
gap(getCX(1920), 1920, 190)

sawDouble(getCX(1500), 1500)
scoreZone(getCX(1350), 1350)

-- Recto final
ramp(getCX(1100), 1100)
gap(getCX(620), 620, 190)

checkpoint(getCX(180), 180)

-- ── JUGADOR ───────────────────────────────────────────────────────────────────
do local cx = getCX(8000)
E[idx] = { components = {
    transform       = { position={x=math.floor(cx-19), y=8000},
                        scale={x=0.55,y=0.55}, rotation=0 },
    sprite          = { asset_id="car_yellow_sprite", width=71, height=131, src_rect={x=0,y=0} },
    rigid_body      = { velocity={x=0,y=0} },
    box_collider    = { width=71, height=131 },
    circle_collider = { radius=35, width=71, height=131 },
    tag             = { tag="player" },
    player          = { base_speed=330, rotation_speed=150, jump_duration=1.7,
                        jump_max_scale=1.7, base_scale_x=0.55, base_scale_y=0.55 },
    nitro           = { cooldown=1.0, boost_duration=1.0 },
    script          = { path="./assets/scripts/player_car.lua" },
}} ; idx = idx + 1
end

-- ── HUD ───────────────────────────────────────────────────────────────────────
E[idx] = { components = {
    transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
    sprite    = { asset_id="hud_bar", width=800, height=40,
                  src_rect={x=0,y=0}, is_fixed=true },
}} ; idx = idx + 1

E[idx] = { components = {
    transform = { position={x=10,y=8}, scale={x=1,y=1}, rotation=0 },
    text  = { text="SCORE: 0", font_asset_id="font_hud",
              color={r=255,g=255,b=80,a=255}, is_fixed=true },
    script = { path="./assets/scripts/hud_score.lua" },
}} ; idx = idx + 1

E[idx] = { components = {
    transform = { position={x=440,y=8}, scale={x=1,y=1}, rotation=0 },
    text  = { text="NITRO: READY [SPC]", font_asset_id="font_hud",
              color={r=80,g=255,b=80,a=255}, is_fixed=true },
    script = { path="./assets/scripts/hud_nitro.lua" },
}} ; idx = idx + 1

E[idx] = { components = {
    transform = { position={x=270,y=50}, scale={x=1,y=1}, rotation=0 },
    text  = { text="LEVEL 3 - FACTORY", font_asset_id="font_hud2",
              color={r=255,g=140,b=80,a=200}, is_fixed=true },
}} ; idx = idx + 1

-- ── ESCENA ────────────────────────────────────────────────────────────────────
scene = {
    sprites = {
        [0] = { assetId="car_yellow_sprite", filePath="./assets/images/car_yellow_1.png" },
    },
    color_textures = {
        [0] = { assetId="road_factory",  r=30,  g=30,  b=35,  a=255 },
        [1] = { assetId="wall_metal",    r=50,  g=50,  b=55,  a=255 },
        [2] = { assetId="saw_red",       r=220, g=40,  b=40,  a=255 },
        [3] = { assetId="saw_orange",    r=255, g=140, b=0,   a=255 },
        [4] = { assetId="gap_void",      r=5,   g=5,   b=10,  a=255 },
        [5] = { assetId="ramp_yellow",   r=255, g=210, b=0,   a=255 },
        [6] = { assetId="checkpoint",    r=240, g=240, b=240, a=255 },
        [7] = { assetId="hud_bar",       r=0,   g=0,   b=0,   a=180 },
        [8] = { assetId="dark_overlay",  r=0,   g=0,   b=0,   a=110 },
    },
    fonts = {
        [0] = { assetId="font_hud",  filePath="./assets/fonts/press_start.ttf", fontSize=18 },
        [1] = { assetId="font_hud2", filePath="./assets/fonts/press_start.ttf", fontSize=14 },
    },
    keys = {
        [0] = { action="accelerate", key=119        },
        [1] = { action="accelerate", key=1073741906 },
        [2] = { action="turn_left",  key=97         },
        [3] = { action="turn_left",  key=1073741904 },
        [4] = { action="turn_right", key=100        },
        [5] = { action="turn_right", key=1073741903 },
        [6] = { action="nitro",      key=32         },
        [7] = { action="nitro",      key=1073742049 },
    },
    buttons  = {},
    entities = E,
}
