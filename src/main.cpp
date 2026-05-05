#include <iostream>

#include "game/game.hpp"

int main(int argc, char* argv[]) {
  (void)argc; // Unused parameter
  (void)argv; // Unused parameter

  std::cout << "Game Engine" << std::endl;

  Game& game = Game::getInstance();
  game.init();
  game.run();
  game.destroy();

  return 0;
}