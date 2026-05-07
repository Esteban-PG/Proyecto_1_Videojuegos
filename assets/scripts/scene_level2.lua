-- scene_level2.lua  —  Nitro Rush: Level 2 — Mountain / Dirt
-- Curvas más pronunciadas, camino más estrecho, tres giros definidos.

current_level = "level_02"
next_level    = "level_03"
zone_points   = 100

local ROAD_W = 260
local WALL_W = 120
local SEG_H  = 70    -- segmentos más pequeños = curvas más definidas

-- Waypoints: curva izquierda → curva derecha → curva izquierda al final
local WP = {
    {y=7200, cx=400},
    {y=6400, cx=400},  -- recto inicial
    {y=5400, cx=240},  -- curva pronunciada a la izquierda
    {y=4400, cx=380},  -- vuelve al centro
    {y=3400, cx=500},  -- curva a la derecha
    {y=2400, cx=380},  -- vuelve al centro
    {y=1400, cx=270},  -- curva leve a la izquierda al final
    {y=0,    cx=270},
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
        sprite    = { asset_id="road_dirt", width=ROAD_W, height=SEG_H+2, src_rect={x=0,y=0} },
    }} ; idx = idx + 1
    E[idx] = { components = {
        transform    = { position={x=rx-WALL_W, y=y}, scale={x=1,y=1}, rotation=0 },
        sprite       = { asset_id="wall_rock", width=WALL_W, height=SEG_H+2, src_rect={x=0,y=0} },
        box_collider = { width=WALL_W, height=SEG_H+2 },
        tag          = { tag="wall" },
    }} ; idx = idx + 1
    E[idx] = { components = {
        transform    = { position={x=rx+ROAD_W, y=y}, scale={x=1,y=1}, rotation=0 },
        sprite       = { asset_id="wall_rock", width=WALL_W, height=SEG_H+2, src_rect={x=0,y=0} },
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

local function sawWall(cx, y, side)
    local w, h, r = 85, 40, 20
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
    local w, h, r = 48, 48, 24
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
    h = h or 200
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
do local y = 7200
   while y >= -SEG_H do
       seg(getCX(y), y)
       y = y - SEG_H
   end
end

-- ── OBSTÁCULOS ────────────────────────────────────────────────────────────────

-- Tramo recto inicial
sawWall(getCX(6600), 6600, "left")
scoreZone(getCX(6450), 6450)

sawCentral(getCX(5900), 5900, 55, 55, 28, false)
scoreZone(getCX(5750), 5750)

-- Curva izquierda (cx≈240)
ramp(getCX(5400), 5400)
gap(getCX(4920), 4920, 200)

sawDouble(getCX(4550), 4550)
scoreZone(getCX(4400), 4400)

-- Centro / curva derecha (cx≈380→500)
ramp(getCX(3800), 3800)
gap(getCX(3320), 3320, 200)

sawCentral(getCX(2900), 2900, 60, 60, 30, true)
scoreZone(getCX(2750), 2750)

sawWall(getCX(2500), 2500, "right")
scoreZone(getCX(2350), 2350)

-- Curva final izquierda (cx≈270)
ramp(getCX(1800), 1800)
gap(getCX(1320), 1320, 200)

sawDouble(getCX(900), 900)
scoreZone(getCX(750), 750)

checkpoint(getCX(180), 180)

-- ── JUGADOR ───────────────────────────────────────────────────────────────────
do local cx = getCX(7000)
E[idx] = { components = {
    transform       = { position={x=math.floor(cx-19), y=7000},
                        scale={x=0.55,y=0.55}, rotation=0 },
    sprite          = { asset_id="car_yellow_sprite", width=71, height=131, src_rect={x=0,y=0} },
    rigid_body      = { velocity={x=0,y=0} },
    box_collider    = { width=71, height=131 },
    circle_collider = { radius=35, width=71, height=131 },
    tag             = { tag="player" },
    player          = { base_speed=310, rotation_speed=140, jump_duration=1.7,
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
    transform = { position={x=265,y=50}, scale={x=1,y=1}, rotation=0 },
    text  = { text="LEVEL 2 - MOUNTAIN", font_asset_id="font_hud2",
              color={r=200,g=160,b=100,a=200}, is_fixed=true },
}} ; idx = idx + 1

-- ── ESCENA ────────────────────────────────────────────────────────────────────
scene = {
    sprites = {
        [0] = { assetId="car_yellow_sprite", filePath="./assets/images/car_yellow_1.png" },
    },
    color_textures = {
        [0] = { assetId="road_dirt",  r=90,  g=65,  b=40,  a=255 },
        [1] = { assetId="wall_rock",  r=55,  g=40,  b=25,  a=255 },
        [2] = { assetId="saw_red",    r=220, g=40,  b=40,  a=255 },
        [3] = { assetId="gap_void",   r=5,   g=5,   b=10,  a=255 },
        [4] = { assetId="ramp_yellow",r=255, g=210, b=0,   a=255 },
        [5] = { assetId="checkpoint", r=240, g=240, b=240, a=255 },
        [6] = { assetId="hud_bar",    r=0,   g=0,   b=0,   a=180 },
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
