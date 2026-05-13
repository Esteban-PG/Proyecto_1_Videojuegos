#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <memory>
#include <sol/sol.hpp>

#include "../ECS/ECS.hpp"
#include "../asset_manager/asset_manager.hpp"
#include "../audio_manager/audio_manager.hpp"
#include "../controller_manager/controller_manager.hpp"
#include "../event_manager/event_manager.hpp"
#include "../scene_manager/scene_manager.hpp"

const int FPS                   = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

/**
 * @brief Central game singleton.
 *
 * Owns all managers, the ECS registry, the Lua VM, and the SDL window/renderer.
 * The main loop runs in run() → runScene() → processInput / update / render.
 *
 * Camera:
 *   cameraX = player.position.x + player_half_w - windowWidth  / 2
 *   cameraY = player.position.y + player_half_h - windowHeight / 2
 *   Updated every frame inside update() after all systems have run.
 *
 * Pause:
 *   ESC toggles isPaused. When paused, update() is skipped and a dark overlay
 *   is drawn over the rendered frame.
 */
class Game {
 private:
  SDL_Window* window            = nullptr;
  int         windowWidth       = 0;
  int         windowHeight      = 0;
  int         millisecondsPreviousFrame = 0;
  bool        isRunning         = false;
  bool        isPaused          = false;
  float       cameraX           = 0.0f;
  float       cameraY           = 0.0f;
  TTF_Font*   pauseFont         = nullptr;

  // Áreas de los botones del menú de pausa (screen space)
  SDL_Rect    pauseBtnResume    = {240, 270, 320, 50};
  SDL_Rect    pauseBtnMenu      = {240, 340, 320, 50};

  void renderPauseButton(const SDL_Rect& rect, const char* label,
                         SDL_Color bg, SDL_Color fg);
  bool pointInRect(int x, int y, const SDL_Rect& r);

 public:
  std::unique_ptr<EventManager>      eventManager;
  std::unique_ptr<AssetManager>      assetManager;
  std::unique_ptr<AudioManager>      audioManager;
  std::unique_ptr<Registry>          registry;
  std::unique_ptr<ControllerManager> controllerManager;
  std::unique_ptr<SceneManager>      sceneManager;
  sol::state lua;

 private:
  void Setup();
  void runScene();
  void processInput();
  void update();
  void render();
  void renderPauseOverlay();
  void updateCamera(float dt);
  Game();
  ~Game();

 public:
  SDL_Renderer* renderer      = nullptr;
  int           mapWidth      = 0;
  int           mapHeight     = 0;
  float         shakeTimer    = 0.0f;
  float         shakeAmplitude = 0.0f;

  static Game& getInstance();
  void triggerShake(float duration, float amplitude);
  void init();
  void run();
  void destroy();
  /** Immediately stop the current scene and exit the game loop. */
  void quit() { isRunning = false; sceneManager->stopScene(); }
};

#endif  // GAME_HPP
