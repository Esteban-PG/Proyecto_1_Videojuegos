#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <map>
#include <string>

/**
 * @brief Loads and caches SDL textures and TTF fonts for the current scene.
 *
 * All assets are cleared between scenes via ClearAssets().
 * Textures can be loaded from file (addTexture), created as solid-colour
 * placeholders (addColorTexture), or injected as pre-built SDL_Texture*
 * objects (addRawTexture — used for baked tile-layer textures).
 */
class AssetManager {
 private:
  std::map<std::string, SDL_Texture*> textures;
  std::map<std::string, TTF_Font*> fonts;

 public:
  AssetManager();
  ~AssetManager();

  /** @brief Destroy all loaded textures and fonts. Called between scenes. */
  void ClearAssets();

  /**
   * @brief Load a PNG/BMP/JPG image from disk and register it under @p textureId.
   * @param renderer   SDL renderer used to create the GPU texture
   * @param textureId  Key used to retrieve the texture later
   * @param filePath   Path to the image file
   */
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

  /**
   * @brief Register an already-created SDL_Texture* (e.g. baked tile layers).
   * @param textureId Key for later retrieval
   * @param texture   Ownership is transferred — AssetManager will destroy it
   */
  void         addRawTexture(const std::string& textureId, SDL_Texture* texture);
  /**
   * @brief @return The texture registered under @p textureId, or nullptr.
   */
  SDL_Texture* getTexture(const std::string& textureId) const;
  /** @brief @return True if a texture with @p textureId is loaded. */
  bool         hasTexture(const std::string& textureId) const;

  /**
   * @brief Load a TTF font at @p fontSize and register it under @p fontId.
   * @param fontId   Key for later retrieval
   * @param filePath Path to the .ttf file
   * @param fontSize Point size to open the font at
   */
  void addFont(const std::string& fontId, const std::string& filePath,
               int fontSize);
  /** @brief @return The font registered under @p fontId, or nullptr. */
  TTF_Font* getFont(const std::string& fontId) const;
  /** @brief @return True if a font with @p fontId is loaded. */
  bool      hasFont(const std::string& fontId) const;
};

#endif  // ASSET_MANAGER_HPP