# Nitro Rush

Top-down car racing game built on a custom C++17 / SDL2 / Lua engine using an Entity-Component-System (ECS) architecture.

---

## Controles

| Tecla                | Acción                         |
| -------------------- | ------------------------------ |
| W / ↑                | Acelerar (mantener presionado) |
| A / ←                | Girar a la izquierda           |
| D / →                | Girar a la derecha             |
| Espacio / Shift izq. | Activar Nitro                  |
| Escape               | Pausar / Reanudar              |

---

## Mecánicas

- **Movimiento**: el carrito solo avanza mientras se mantiene presionada la tecla de avance; al soltar se detiene.
- **Nitro**: boost de velocidad (×1.6) con cooldown de 2 segundos. Indicador visual en HUD.
- **Saltos**: activa nitro antes o encima de una rampa (amarilla) para saltar el hueco; sin nitro = muerte instantánea.
- **Sierras**: 4 tipos (pared, central, doble, giratoria). Tocarlas = muerte. Esquivarlas = puntos.
- **Paredes**: rebotan levemente el carrito sin daño.
- **Checkpoint** (blanco): cruzarlo = completar el nivel.

---

## Niveles

| Nivel | Escenario        | Dificultad | Duración aprox. |
| ----- | ---------------- | ---------- | --------------- |
| 1     | Ciudad / Asfalto | Baja       | ~45 s           |
| 2     | Montaña / Tierra | Media      | ~60 s           |
| 3     | Fábrica / Noche  | Alta       | ~75 s           |

La lógica de cada nivel está en `assets/scripts/scene_levelN.lua` — editable sin recompilar.

---

## Requisitos

| Dependencia | Versión |
| ----------- | ------- |
| g++         | C++17   |
| SDL2        | 2.x     |
| SDL2_image  | 2.x     |
| SDL2_ttf    | 2.x     |
| SDL2_mixer  | 2.x     |
| Lua         | 5.4     |

En macOS con Homebrew:

```bash
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer lua@5.4
```

---

## Compilar y ejecutar

```bash
# Compilar
make build

# Ejecutar
make run

# Limpiar binarios
make clean
```

---

## Audio

Los archivos de sonido van en `assets/sounds/`. El sistema busca estos IDs:

| ID             | Descripción               |
| -------------- | ------------------------- |
| `engine`       | Motor del carrito (loop)  |
| `nitro`        | Activación del nitro      |
| `jump`         | Salto en rampa            |
| `bounce`       | Rebote contra pared       |
| `death`        | Muerte del jugador        |
| `win`          | Llegar al checkpoint      |
| `music_menu`   | Música del menú principal |
| `music_level1` | Música nivel 1            |
| `music_level2` | Música nivel 2            |
| `music_level3` | Música nivel 3            |

Si un archivo no existe el motor lo ignora y continúa. Formatos soportados: WAV, OGG, MP3.

---

## Estructura de assets para sprites reales

```
assets/
├── fonts/
│   └── press_start.ttf
├── images/
│   ├── car/car_blue.png
│   ├── obstacles/saw_*.png
│   ├── track/road_*.png
│   └── ui/ramp.png, checkpoint.png
├── sounds/
│   ├── engine.wav
│   ├── nitro.wav
│   └── ...
└── scripts/
    ├── scenes.lua
    ├── scene_menu.lua
    ├── scene_level1.lua
    ├── scene_level2.lua
    ├── scene_level3.lua
    ├── player_car.lua
    ├── saw_spin.lua
    ├── score_zone.lua
    ├── hud_score.lua
    └── hud_nitro.lua
```

Para reemplazar un placeholder cambia `color_textures` por `sprites` en la escena y apunta al PNG.

---

## Arquitectura — Clases nuevas y modificadas

### Clases nuevas

| Archivo                                 | Clase                  | Responsabilidad                                              |
| --------------------------------------- | ---------------------- | ------------------------------------------------------------ |
| `components/tag_component.hpp`          | `TagComponent`         | Etiqueta de rol (player, saw, wall, gap, ramp, checkpoint)   |
| `components/box_collider_component.hpp` | `BoxColliderComponent` | AABB para paredes/huecos/rampas/checkpoint                   |
| `components/nitro_component.hpp`        | `NitroComponent`       | Estado y cooldown del nitro                                  |
| `components/player_component.hpp`       | `PlayerComponent`      | Estado del carrito (velocidad, salto, puntos, muerte)        |
| `audio_manager/audio_manager.hpp/.cpp`  | `AudioManager`         | Wrapper SDL_mixer para música y efectos                      |
| `systems/car_movement_system.hpp`       | `CarMovementSystem`    | Movimiento angular (input → velocidad con ángulo)            |
| `systems/nitro_system.hpp`              | `NitroSystem`          | Ciclo boost/cooldown + activación por input                  |
| `systems/box_collision_system.hpp`      | `BoxCollisionSystem`   | Pared→rebote, hueco→muerte, rampa→salto, checkpoint→victoria |
| `systems/tag_system.hpp`                | `TagSystem`            | Lookup rápido de entidades por tag                           |

### Clases modificadas

| Archivo                                | Cambios principales                                                            |
| -------------------------------------- | ------------------------------------------------------------------------------ |
| `game/game.hpp`                        | `AudioManager`, cámara (cameraX/Y), pausa (isPaused), `quit()`                 |
| `game/game.cpp`                        | Nuevos sistemas, bucle con pausa, `updateCamera()`, overlay pausa, globals Lua |
| `systems/render_system.hpp`            | Offset de cámara + flag `isFixed` para HUD                                     |
| `systems/render_text_system.hpp`       | Offset de cámara + flag `isFixed` para HUD                                     |
| `systems/damage_system.hpp`            | Tag-aware: solo mata al jugador al tocar sierra; sierras sobreviven            |
| `components/text_component.hpp`        | Campo `isFixed` (HUD fijo en pantalla)                                         |
| `components/sprite_component.hpp`      | Campo `isFixed`                                                                |
| `asset_manager/asset_manager.hpp/.cpp` | `addColorTexture()`, `hasFont()`, `hasTexture()`                               |
| `scene_manager/scene_loader.hpp/.cpp`  | Carga `color_textures`, `box_collider`, `tag`, `nitro`, `player`               |
| `binding/lua_binding.hpp`              | Nuevas funciones: transform, player, HUD, audio, escenas                       |
| `systems/script_system.hpp`            | Registra todas las funciones Lua nuevas                                        |
| `ECS/ECS.hpp`                          | Constructor por defecto `Entity()` para uso en `unordered_map`                 |
| `makefile`                             | `-lSDL2_mixer`, directorio `audio_manager`                                     |

---

## Funciones Lua expuestas a scripts

```lua
-- Input
isActionActivated(action)          -> bool

-- Transform
getEntityPosition(entity)          -> x, y
setEntityPosition(entity, x, y)
getEntityRotation(entity)          -> degrees
setEntityRotation(entity, degrees)

-- RigidBody
setVelocity(entity, x, y)
getVelocity(entity)                -> x, y

-- Player (busca tag "player" via TagSystem)
getPlayerPosition()                -> x, y
isPlayerDead(entity)               -> bool
isPlayerWon(entity)                -> bool
addScore(value)
getScore()                         -> int

-- HUD
setTextContent(entity, text)

-- Audio
playSFX(id)
playMusic(id)
stopMusic()

-- Escenas
goToScene(name)
quitGame()

-- Globales inyectadas por game.cpp cada frame:
delta_time           -- float (segundos del frame)
current_score        -- int
nitro_cooldown       -- float (segundos restantes)
nitro_max_cooldown   -- float (cooldown total configurado)
nitro_ready          -- bool
```
