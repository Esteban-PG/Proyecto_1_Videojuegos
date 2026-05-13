#include "game.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>

#include "../events/click_event.hpp"
#include "../systems/animation_system.hpp"
#include "../systems/box_collision_system.hpp"
#include "../systems/car_movement_system.hpp"
#include "../systems/collision_system.hpp"
#include "../systems/damage_system.hpp"
#include "../systems/movement_system.hpp"
#include "../systems/nitro_system.hpp"
#include "../systems/render_system.hpp"
#include "../systems/render_text_system.hpp"
#include "../systems/script_system.hpp"
#include "../systems/tag_system.hpp"
#include "../systems/ui_system.hpp"
#include "../components/player_component.hpp"
#include "../components/nitro_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/transform_component.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

Game::Game() {
  std::cout << "[GAME] Constructor called" << std::endl;
  assetManager      = std::make_unique<AssetManager>();
  audioManager      = std::make_unique<AudioManager>();
  eventManager      = std::make_unique<EventManager>();
  controllerManager = std::make_unique<ControllerManager>();
  registry          = std::make_unique<Registry>();
  sceneManager      = std::make_unique<SceneManager>();
}

Game::~Game() {
  std::cout << "[GAME] Destructor called" << std::endl;
  registry.reset();
  assetManager.reset();
  audioManager.reset();
  controllerManager.reset();
  eventManager.reset();
  sceneManager.reset();
}

// ─────────────────────────────────────────────────────────────────────────────
// Singleton
// ─────────────────────────────────────────────────────────────────────────────

Game& Game::getInstance() {
  static Game instance;
  return instance;
}

// ─────────────────────────────────────────────────────────────────────────────
// Initialisation
// ─────────────────────────────────────────────────────────────────────────────

void Game::init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "[GAME] SDL_Init error: " << SDL_GetError() << std::endl;
    return;
  }
  if (TTF_Init() != 0) {
    std::cerr << "[GAME] TTF_Init error: " << TTF_GetError() << std::endl;
    return;
  }

  windowWidth  = 800;
  windowHeight = 600;

  window = SDL_CreateWindow("Nitro Rush", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                            SDL_WINDOW_SHOWN);
  if (!window) {
    std::cerr << "[GAME] CreateWindow error: " << SDL_GetError() << std::endl;
    return;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "[GAME] CreateRenderer error: " << SDL_GetError() << std::endl;
    return;
  }

  // Initialise audio (non-fatal if it fails)
  audioManager->init();

  isRunning = true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Setup (run once before the main scene loop)
// ─────────────────────────────────────────────────────────────────────────────

void Game::Setup() {
  // Order matters: some systems depend on others being present
  registry->addSystem<TagSystem>();
  registry->addSystem<CollisionSystem>();
  registry->addSystem<BoxCollisionSystem>();
  registry->addSystem<RenderSystem>();
  registry->addSystem<MovementSystem>();
  registry->addSystem<CarMovementSystem>();
  registry->addSystem<NitroSystem>();
  registry->addSystem<DamageSystem>();
  registry->addSystem<AnimationSystem>();
  registry->addSystem<ScriptSystem>();
  registry->addSystem<RenderTextSystem>();
  registry->addSystem<UISystem>();

  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
  registry->getSystem<ScriptSystem>().createLuaBinding(lua);

  // Auto-cargar audio: intenta .ogg, .mp3, .wav — falla silenciosamente si no existe
  auto tryMusic = [&](const std::string& id) {
    for (const char* ext : {".ogg", ".mp3", ".wav"}) {
      std::string path = "./assets/sounds/" + id + ext;
      if (FILE* f = fopen(path.c_str(), "r")) { fclose(f);
        audioManager->addMusic(id, path); return; }
    }
  };
  auto trySFX = [&](const std::string& id) {
    for (const char* ext : {".wav", ".ogg", ".mp3"}) {
      std::string path = "./assets/sounds/" + id + ext;
      if (FILE* f = fopen(path.c_str(), "r")) { fclose(f);
        audioManager->addSFX(id, path); return; }
    }
  };

  tryMusic("music_menu");
  tryMusic("music_level1");
  tryMusic("music_level2");
  tryMusic("music_level3");

  trySFX("jump");
  trySFX("bounce");
  trySFX("death");
  trySFX("win");

  sceneManager->loadScenesFromScript("./assets/scripts/scenes.lua", lua);
}

// ─────────────────────────────────────────────────────────────────────────────
// Input processing
// ─────────────────────────────────────────────────────────────────────────────

void Game::processInput() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
      case SDL_QUIT:
        sceneManager->stopScene();
        isRunning = false;
        break;

      case SDL_KEYDOWN:
        if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
          // Toggle pause (hold Escape on menu to quit gracefully)
          isPaused = !isPaused;
          break;
        }
        controllerManager->keyDown(sdlEvent.key.keysym.sym);
        break;

      case SDL_KEYUP:
        controllerManager->keyUp(sdlEvent.key.keysym.sym);
        break;

      case SDL_MOUSEMOTION: {
        int x, y;
        SDL_GetMouseState(&x, &y);
        controllerManager->setMousePosition(x, y);
        break;
      }
      case SDL_MOUSEBUTTONDOWN:
        if (isWinScreen && sdlEvent.button.button == SDL_BUTTON_LEFT) {
          int mx = sdlEvent.button.x, my = sdlEvent.button.y;
          if (pointInRect(mx, my, winBtnNext)) {
            isWinScreen = false;
            std::string next = lua["next_level"].get_or<std::string>("nitro_menu");
            sceneManager->setNextScene(next);
            sceneManager->stopScene();
          }
          break;
        }
        if (isPaused && sdlEvent.button.button == SDL_BUTTON_LEFT) {
          int mx = sdlEvent.button.x, my = sdlEvent.button.y;
          if (pointInRect(mx, my, pauseBtnResume)) {
            isPaused = false;
          } else if (pointInRect(mx, my, pauseBtnMenu)) {
            isPaused = false;
            sceneManager->setNextScene("nitro_menu");
            sceneManager->stopScene();
          }
          break;
        }
        controllerManager->setMousePosition(sdlEvent.button.x, sdlEvent.button.y);
        controllerManager->mouseButtonDown(static_cast<int>(sdlEvent.button.button));
        eventManager->emitEvent<ClickEvent>(
            static_cast<int>(sdlEvent.button.button),
            sdlEvent.button.x, sdlEvent.button.y);
        break;

      case SDL_MOUSEBUTTONUP:
        controllerManager->setMousePosition(sdlEvent.button.x, sdlEvent.button.y);
        controllerManager->mouseButtonUp(static_cast<int>(sdlEvent.button.button));
        break;

      default:
        break;
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Camera update (called at end of update())
// ─────────────────────────────────────────────────────────────────────────────

void Game::triggerShake(float duration, float amplitude) {
  shakeTimer     = duration;
  shakeAmplitude = amplitude;
}

void Game::updateCamera(float dt) {
  auto& tagSys = registry->getSystem<TagSystem>();
  Entity player = tagSys.getEntityByTag("player");
  if (player.getId() < 0) return;
  if (!player.hasComponent<TransformComponent>()) return;

  const auto& tf = player.getComponent<TransformComponent>();
  float halfW = 15.0f;
  float halfH = 25.0f;

  if (player.hasComponent<SpriteComponent>()) {
    const auto& sp = player.getComponent<SpriteComponent>();
    halfW = (sp.width  * static_cast<float>(tf.scale.x)) * 0.5f;
    halfH = (sp.height * static_cast<float>(tf.scale.y)) * 0.5f;
  }

  cameraX = tf.position.x + halfW - windowWidth  * 0.5f;
  cameraY = tf.position.y + halfH - windowHeight * 0.5f;

  // Camera shake: decrement timer y aplicar offset aleatorio
  if (shakeTimer > 0.0f) {
    shakeTimer -= dt;
    if (shakeTimer < 0.0f) shakeTimer = 0.0f;

    int amp = static_cast<int>(shakeAmplitude);
    if (amp > 0) {
      cameraX += static_cast<float>(rand() % (2 * amp + 1) - amp);
      cameraY += static_cast<float>(rand() % (2 * amp + 1) - amp);
    }
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Update
// ─────────────────────────────────────────────────────────────────────────────

void Game::update() {
  // Frame-rate cap
  int timeToWait = MILLISECONDS_PER_FRAME -
                   static_cast<int>(SDL_GetTicks() - millisecondsPreviousFrame);
  if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME)
    SDL_Delay(timeToWait);

  double deltaTime =
      (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0;
  millisecondsPreviousFrame = SDL_GetTicks();

  // Expose deltaTime to Lua so scripts can do frame-rate independent work
  lua["delta_time"] = static_cast<float>(deltaTime);

  // Reset per-frame event subscriptions
  eventManager->reset();
  registry->getSystem<DamageSystem>().subscribeToCollisionEvent(eventManager);
  registry->getSystem<UISystem>().subscribeToClickEvent(eventManager);

  registry->update();

  // ── Game systems in dependency order ───────────────────────────────────────
  registry->getSystem<TagSystem>().update();

  // Update score / nitro globals for Lua HUD scripts
  {
    auto& tagSys = registry->getSystem<TagSystem>();
    Entity player = tagSys.getEntityByTag("player");
    lua["current_score"]    = 0;
    lua["nitro_cooldown"]   = 0.0f;
    lua["nitro_max_cooldown"] = 2.0f;
    lua["nitro_ready"]      = true;

    if (player.getId() >= 0) {
      if (player.hasComponent<PlayerComponent>()) {
        lua["current_score"] =
            player.getComponent<PlayerComponent>().score;
      }
      if (player.hasComponent<NitroComponent>()) {
        const auto& n = player.getComponent<NitroComponent>();
        lua["nitro_cooldown"]     = n.cooldownRemaining;
        lua["nitro_max_cooldown"] = n.cooldownTime;
        lua["nitro_ready"]        = !n.isOnCooldown;
      }
    }
  }

  registry->getSystem<ScriptSystem>().update(lua);
  registry->getSystem<NitroSystem>().update(deltaTime);
  registry->getSystem<CarMovementSystem>().update(deltaTime);
  registry->getSystem<MovementSystem>().update(deltaTime);
  registry->getSystem<BoxCollisionSystem>().update();
  registry->getSystem<CollisionSystem>().update(eventManager);
  registry->getSystem<AnimationSystem>().update();

  updateCamera(static_cast<float>(deltaTime));
}

// ─────────────────────────────────────────────────────────────────────────────
// Render
// ─────────────────────────────────────────────────────────────────────────────

void Game::render() {
  SDL_SetRenderDrawColor(renderer, 20, 20, 28, 255);
  SDL_RenderClear(renderer);

  registry->getSystem<RenderSystem>().update(renderer, assetManager, cameraX, cameraY);
  registry->getSystem<RenderTextSystem>().update(renderer, assetManager, cameraX, cameraY);

  if (isPaused)    renderPauseOverlay();
  if (isWinScreen) renderWinScreen();

  SDL_RenderPresent(renderer);
}

void Game::renderWinScreen() {
  if (!pauseFont)
    pauseFont = TTF_OpenFont("./assets/fonts/press_start.ttf", 16);

  // Overlay oscuro
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
  SDL_Rect overlay = {0, 0, windowWidth, windowHeight};
  SDL_RenderFillRect(renderer, &overlay);

  // Panel
  SDL_Rect panel = {220, 170, 360, 260};
  SDL_SetRenderDrawColor(renderer, 10, 40, 10, 245);
  SDL_RenderFillRect(renderer, &panel);

  // Línea dorada superior
  SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
  SDL_Rect line = {220, 170, 360, 3};
  SDL_RenderFillRect(renderer, &line);

  if (pauseFont) {
    // Título
    {
      SDL_Surface* s = TTF_RenderText_Blended(pauseFont, "NIVEL COMPLETO!", {255, 220, 0, 255});
      if (s) {
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect d = {windowWidth/2 - s->w/2, 192, s->w, s->h};
        SDL_FreeSurface(s); SDL_RenderCopy(renderer, t, nullptr, &d);
        SDL_DestroyTexture(t);
      }
    }
    // Puntuación
    {
      int score = lua["current_score"].get_or(0);
      std::string scoreStr = "SCORE: " + std::to_string(score);
      SDL_Surface* s = TTF_RenderText_Blended(pauseFont, scoreStr.c_str(), {200, 255, 200, 255});
      if (s) {
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect d = {windowWidth/2 - s->w/2, 270, s->w, s->h};
        SDL_FreeSurface(s); SDL_RenderCopy(renderer, t, nullptr, &d);
        SDL_DestroyTexture(t);
      }
    }
  }

  // Botón SIGUIENTE NIVEL
  renderPauseButton(winBtnNext, "SIGUIENTE NIVEL",
                    {20, 100, 20, 255}, {255, 255, 255, 255});
}

bool Game::pointInRect(int x, int y, const SDL_Rect& r) {
  return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h;
}

void Game::renderPauseButton(const SDL_Rect& rect, const char* label,
                              SDL_Color bg, SDL_Color fg) {
  // Fondo del botón
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
  SDL_RenderFillRect(renderer, &rect);

  if (!pauseFont) return;
  SDL_Surface* surf = TTF_RenderText_Blended(pauseFont, label, fg);
  if (!surf) return;
  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_Rect dst = {
      rect.x + (rect.w - surf->w) / 2,
      rect.y + (rect.h - surf->h) / 2,
      surf->w, surf->h
  };
  SDL_FreeSurface(surf);
  SDL_RenderCopy(renderer, tex, nullptr, &dst);
  SDL_DestroyTexture(tex);
}

void Game::renderPauseOverlay() {
  // Cargar fuente la primera vez
  if (!pauseFont)
    pauseFont = TTF_OpenFont("./assets/fonts/press_start.ttf", 16);

  // Overlay oscuro
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 170);
  SDL_Rect overlay = {0, 0, windowWidth, windowHeight};
  SDL_RenderFillRect(renderer, &overlay);

  // Panel central
  SDL_Rect panel = {220, 190, 360, 230};
  SDL_SetRenderDrawColor(renderer, 12, 12, 30, 240);
  SDL_RenderFillRect(renderer, &panel);

  // Línea dorada superior del panel
  SDL_SetRenderDrawColor(renderer, 255, 200, 0, 200);
  SDL_Rect line = {220, 190, 360, 3};
  SDL_RenderFillRect(renderer, &line);

  // Título "PAUSED"
  if (pauseFont) {
    SDL_Surface* surf = TTF_RenderText_Blended(pauseFont, "PAUSED",
                                               {255, 200, 0, 255});
    if (surf) {
      SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
      SDL_Rect dst = {windowWidth / 2 - surf->w / 2, 210, surf->w, surf->h};
      SDL_FreeSurface(surf);
      SDL_RenderCopy(renderer, tex, nullptr, &dst);
      SDL_DestroyTexture(tex);
    }
  }

  // Botón RESUME
  renderPauseButton(pauseBtnResume, "RESUME",
                    {30, 90, 30, 255}, {255, 255, 255, 255});

  // Botón MAIN MENU
  renderPauseButton(pauseBtnMenu, "MAIN MENU",
                    {60, 20, 20, 255}, {255, 180, 180, 255});
}

// ─────────────────────────────────────────────────────────────────────────────
// Scene loop
// ─────────────────────────────────────────────────────────────────────────────

void Game::runScene() {
  isPaused = false;
  cameraX  = 0.0f;
  cameraY  = 0.0f;

  isWinScreen = false;
  sceneManager->loadScene();

  while (sceneManager->isSceneRunning()) {
    processInput();
    if (!isPaused && !isWinScreen) {
      update();
    }
    render();
  }

  audioManager->stopMusic(0);
  assetManager->ClearAssets();
  registry->clearAllEntities();
}

// ─────────────────────────────────────────────────────────────────────────────
// Main game loop
// ─────────────────────────────────────────────────────────────────────────────

void Game::run() {
  Setup();
  while (isRunning) {
    sceneManager->startScene();
    runScene();
  }
}

// ─────────────────────────────────────────────────────────────────────────────
// Cleanup
// ─────────────────────────────────────────────────────────────────────────────

void Game::destroy() {
  if (assetManager) assetManager->ClearAssets();
  if (audioManager) audioManager->destroy();

  if (pauseFont) { TTF_CloseFont(pauseFont);       pauseFont = nullptr; }
  if (renderer)  { SDL_DestroyRenderer(renderer); renderer  = nullptr; }
  if (window)    { SDL_DestroyWindow(window);      window    = nullptr; }

  Mix_Quit();
  TTF_Quit();
  SDL_Quit();
}
