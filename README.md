# Nitro Rush

Juego de carreras top-down construido sobre un motor custom en C++17 / SDL2 / Lua con arquitectura Entity-Component-System (ECS). Los niveles se diseñan en **Tiled** y la lógica de juego se controla desde **scripts Lua** sin necesidad de recompilar.

---

## Controles

| Tecla                  | Acción                  |
| ---------------------- | ----------------------- |
| W / ↑                  | Acelerar                |
| A / ←                  | Girar a la izquierda    |
| D / →                  | Girar a la derecha      |
| Espacio / Shift izq.   | Activar Nitro           |
| ESC                    | Pausar / Reanudar       |

---

## Mecánicas

- **Movimiento**: el carrito avanza mientras se mantiene W/↑; al soltar frena progresivamente.
- **Nitro**: boost de velocidad ×2.5 con duración de 0.5 s y cooldown de 0.5 s. Indicador en HUD.
- **Salto**: activa el nitro justo antes o encima de una rampa para saltar un hueco. Sin nitro el carrito cae y muere con animación de encogimiento.
- **Animación de caída**: al caer en un hueco, el carrito se desliza hacia el centro del hueco mientras se encoge y desaparece, luego reaparece en el spawn sin recargar el nivel.
- **Sierras estáticas**: bloques con colisión de caja. Tocarlas = muerte.
- **Sierras patrulla** (`patrol_saw`): sierras con imagen propia que se mueven en un eje (X o Y) de forma continua y giran sobre sí mismas. Configurables desde Tiled con propiedades `patrol_axis`, `patrol_range`, `patrol_speed`.
- **Paredes**: rebotan levemente el carrito sin daño.
- **Checkpoint**: cruzarlo muestra la pantalla de victoria con el score y el botón "Siguiente Nivel".
- **Score**: +100 puntos al pasar por encima de una sierra sin morir; +75 al saltar con nitro en una rampa.
- **Shake de cámara**: al despegar y al aterrizar de un salto.

---

## Menú principal

- Selección de nivel (LVL 01 / LVL 02 / LVL 03)
- Botón QUIT

## Menú de pausa (ESC en nivel)

- **RESUME** — reanuda el juego
- **MAIN MENU** — vuelve al menú principal

## Pantalla de victoria

Aparece al cruzar el checkpoint con el score acumulado y el botón **SIGUIENTE NIVEL**.

---

## Niveles

Los niveles se crean en **Tiled** (archivos `.tmx`) y se conectan al motor mediante un script Lua mínimo por nivel.

| Nivel   | Archivo TMX          | Script Lua                    |
| ------- | -------------------- | ----------------------------- |
| LVL 01  | `assets/map/lvl_01.tmx` | `assets/scripts/scene_tiled_01.lua` |
| LVL 02  | `assets/map/lvl_02.tmx` | `assets/scripts/scene_tiled_02.lua` |
| LVL 03  | `assets/map/lvl_03.tmx` | `assets/scripts/scene_tiled_03.lua` |

### Tipos de objetos soportados en el Object Layer de Tiled

| Class en Tiled    | Resultado en el juego                              |
| ----------------- | -------------------------------------------------- |
| `wall`            | Colisión de rebote                                 |
| `gap`             | Hueco — caída con animación                        |
| `ramp`            | Rampa — permite salto con nitro                    |
| `saw`             | Sierra letal estática                              |
| `patrol_saw`      | Sierra letal con movimiento de patrulla y rotación |
| `score_zone`      | Zona invisible que otorga puntos al pasar          |
| `checkpoint`      | Meta del nivel                                     |
| `player_spawn`    | Posición inicial del jugador                       |

#### Propiedades custom de `patrol_saw`

| Propiedad      | Tipo   | Default | Descripción                        |
| -------------- | ------ | ------- | ---------------------------------- |
| `patrol_axis`  | string | `"y"`   | Eje de movimiento (`"x"` o `"y"`)  |
| `patrol_range` | float  | `80`    | Distancia máxima desde el origen   |
| `patrol_speed` | float  | `60`    | Velocidad en px/s                  |
| `patrol_spin`  | float  | `200`   | Velocidad de rotación en °/s       |

#### Propiedades custom de `player_spawn`

| Propiedad          | Tipo  | Default |
| ------------------ | ----- | ------- |
| `base_speed`       | float | `220`   |
| `rotation_speed`   | float | `130`   |
| `scale`            | float | `0.35`  |

---

## Audio

Coloca los archivos en `assets/sounds/` con los nombres exactos. El motor los carga automáticamente al iniciar; si no existen, el juego continúa sin ese sonido.

| Archivo             | Descripción               | Formatos      |
| ------------------- | ------------------------- | ------------- |
| `music_menu.ogg`    | Música menú principal     | ogg / mp3 / wav |
| `music_level1.ogg`  | Música nivel 1            | ogg / mp3 / wav |
| `music_level2.ogg`  | Música nivel 2            | ogg / mp3 / wav |
| `music_level3.ogg`  | Música nivel 3            | ogg / mp3 / wav |
| `jump.wav`          | Efecto salto              | wav / ogg     |
| `bounce.wav`        | Efecto rebote en pared    | wav / ogg     |
| `death.wav`         | Efecto muerte             | wav / ogg     |
| `win.wav`           | Efecto checkpoint         | wav / ogg     |

> La música se reproduce en loop al 50% de volumen. Los efectos de sonido (SFX) requieren `.wav` u `.ogg` — MP3 no funciona para SFX con SDL_mixer.

Para convertir MP3 a WAV con ffmpeg:
```bash
ffmpeg -i archivo.mp3 assets/sounds/archivo.wav
```

---

## Requisitos

| Dependencia  | Versión |
| ------------ | ------- |
| g++          | C++17   |
| SDL2         | 2.x     |
| SDL2_image   | 2.x     |
| SDL2_ttf     | 2.x     |
| SDL2_mixer   | 2.x     |
| Lua          | 5.4     |

En macOS con Homebrew:
```bash
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer lua@5.4
```

---

## Compilar y ejecutar

```bash
make build   # Compilar
make run     # Ejecutar
make clean   # Limpiar binarios
```

---

## Estructura del proyecto

```
assets/
├── fonts/
│   └── press_start.ttf
├── images/
│   ├── Terrain.png          ← tileset principal
│   ├── car_yellow_1.png
│   └── saw_a.png
├── map/
│   ├── lvl_01.tmx
│   ├── lvl_02.tmx
│   ├── lvl_03.tmx
│   └── terrain.tsx
├── sounds/
│   └── (coloca aquí los archivos de audio)
└── scripts/
    ├── scenes.lua
    ├── scene_menu.lua
    ├── scene_tiled_01.lua
    ├── scene_tiled_02.lua
    ├── scene_tiled_03.lua
    ├── player_car.lua
    ├── saw_spin.lua
    ├── saw_patrol.lua
    ├── score_zone.lua
    ├── hud_score.lua
    └── hud_nitro.lua
src/
├── ECS/                     ← Entity, Component, System, Registry
├── asset_manager/
├── audio_manager/           ← SDL_mixer wrapper
├── binding/                 ← Funciones Lua expuestas al motor
├── components/              ← 11 tipos de componentes
├── controller_manager/
├── event_manager/
├── events/
├── game/                    ← Game singleton, loop principal, cámara, pausa
├── scene_manager/           ← SceneLoader (Lua + TMX vía tinyxml2)
├── systems/                 ← 11 sistemas ECS
└── utils/                   ← Pool<T>
libs/
├── sol/                     ← Bindings Lua/C++
├── glm/                     ← Matemáticas
├── lua/                     ← Headers Lua 5.4
└── tinyxml2/                ← Parser TMX
```

---

## Funciones Lua disponibles en scripts

```lua
-- Input
isActionActivated(action)           -> bool

-- Transform
getEntityPosition(entity)           -> x, y
setEntityPosition(entity, x, y)
getEntityRotation(entity)           -> degrees
setEntityRotation(entity, degrees)

-- Física
setVelocity(entity, vx, vy)
getVelocity(entity)                 -> vx, vy

-- Jugador
getPlayerPosition()                 -> x, y
isPlayerDead(entity)                -> bool
isPlayerWon(entity)                 -> bool
respawnPlayer()                     -- respawn sin recargar escena
triggerWin()                        -- muestra pantalla de victoria
addScore(value)
getScore()                          -> int

-- HUD
setTextContent(entity, text)

-- Audio
playSFX(id)
playMusic(id)
stopMusic()

-- Escenas
goToScene(name)
quitGame()

-- Globales por frame (inyectadas desde game.cpp)
delta_time           -- float: segundos del frame
current_score        -- int
nitro_cooldown       -- float: segundos restantes del cooldown
nitro_max_cooldown   -- float: duración total del cooldown
nitro_ready          -- bool
```

---

## Agregar un nivel nuevo

1. Crea el mapa en Tiled (tileset 16×16, mapa 50 tiles de ancho).
2. Agrega un **Object Layer** con los objetos del nivel (ver tabla de clases arriba).
3. Exporta como `.tmx` en `assets/map/`.
4. Copia `scene_tiled_01.lua`, renómbralo y apunta `map.path` al nuevo `.tmx`.
5. Regístralo en `assets/scripts/scenes.lua`.
6. Actualiza `next_level` en el nivel anterior para encadenarlos.
