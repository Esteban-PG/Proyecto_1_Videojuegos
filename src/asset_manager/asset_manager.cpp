#include "asset_manager.hpp"

#include <iostream>

#include "SDL2/SDL_image.h"

AssetManager::AssetManager() {
  std::cout << "[ASSET MANAGER] Constructor called" << std::endl;
}

AssetManager::~AssetManager() {
  std::cout << "[ASSET MANAGER] Destructor called" << std::endl;
  ClearAssets();
}

void AssetManager::ClearAssets() {
  std::cout << "[ASSET MANAGER] Clearing assets" << std::endl;
  for (auto& pair : textures) {
    if (pair.second) {
      SDL_DestroyTexture(pair.second);
    }
  }
  textures.clear();

  for (auto& pair : fonts) {
    if (pair.second) {
      TTF_CloseFont(pair.second);
    }
  }
  fonts.clear();
}

void AssetManager::addTexture(SDL_Renderer* renderer,
                              const std::string& textureId,
                              const std::string& filePath) {
  std::cout << "[ASSET MANAGER] Adding texture: " << textureId << std::endl;
  SDL_Surface* surface = IMG_Load(filePath.c_str());
  if (!surface) {
    std::cerr << "[ASSET MANAGER] Failed to load texture: " << filePath
              << std::endl;
    return;
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (!texture) {
    std::cerr << "[ASSET MANAGER] Failed to create texture: " << textureId
              << std::endl;
    return;
  }
  textures.emplace(textureId, texture);
}

void AssetManager::addColorTexture(SDL_Renderer* renderer,
                                   const std::string& textureId,
                                   Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  // 4×4 surface gives cleaner edges than 1×1 when SDL scales with filtering
  SDL_Surface* surface = SDL_CreateRGBSurface(0, 4, 4, 32,
    0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
  if (!surface) return;

  SDL_FillRect(surface, nullptr,
               SDL_MapRGBA(surface->format, r, g, b, a));

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (!texture) return;

  // Allow alpha blending so gap/HUD overlays work correctly
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  textures.emplace(textureId, texture);
  std::cout << "[ASSET MANAGER] Added colour texture: " << textureId << std::endl;
}

void AssetManager::addRawTexture(const std::string& textureId,
                                  SDL_Texture* texture) {
  if (texture) textures.emplace(textureId, texture);
}

SDL_Texture* AssetManager::getTexture(const std::string& textureId) const {
  auto it = textures.find(textureId);
  if (it != textures.end()) {
    return it->second;
  }
  std::cerr << "[ASSET MANAGER] Texture not found: " << textureId << std::endl;
  return nullptr;
}

void AssetManager::addFont(const std::string& fontId,
                           const std::string& filePath, int fontSize) {
  TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
  if (!font) {
    std::cerr << "[ASSET MANAGER] Failed to load font: " << filePath
              << std::endl;
    return;
  }
  fonts.emplace(fontId, font);
}

TTF_Font* AssetManager::getFont(const std::string& fontId) const {
  auto it = fonts.find(fontId);
  if (it != fonts.end()) {
    return it->second;
  }
  std::cerr << "[ASSET MANAGER] Font not found: " << fontId << std::endl;
  return nullptr;
}

bool AssetManager::hasFont(const std::string& fontId) const {
  return fonts.find(fontId) != fonts.end();
}

bool AssetManager::hasTexture(const std::string& textureId) const {
  return textures.find(textureId) != textures.end();
}