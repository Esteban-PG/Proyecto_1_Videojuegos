#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <map>
#include <string>

class AssetManager {
 private:
  std::map<std::string, SDL_Texture*> textures;
  std::map<std::string, TTF_Font*> fonts;

 public:
  AssetManager();
  ~AssetManager();

  void ClearAssets();

  void addTexture(SDL_Renderer* renderer, const std::string& textureId,
                  const std::string& filePath);

  /**
   * @brief Create a solid-colour 4×4 texture that can be stretched to any size.
   *
   * Useful for placeholder sprites (road, walls, saws, HUD elements) when no
   * real art file is available. The texture supports alpha blending.
   */
  void addColorTexture(SDL_Renderer* renderer, const std::string& textureId,
                       Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

  SDL_Texture* getTexture(const std::string& textureId) const;
  bool         hasTexture(const std::string& textureId) const;

  void addFont(const std::string& fontId, const std::string& filePath,
               int fontSize);
  TTF_Font* getFont(const std::string& fontId) const;
  bool      hasFont(const std::string& fontId) const;
};

#endif  // ASSET_MANAGER_HPP