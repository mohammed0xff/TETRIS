#pragma once 

#include <iostream>
#include <string>


// DISPLAY VARIABLES
constexpr std::size_t SCREEN_WIDTH{ 1000 };
constexpr std::size_t SCREEN_HEIGHT{ 750 };
constexpr std::size_t FPS{ 60 };
constexpr std::size_t MsPerFrame{ 1000 / FPS };

// GAME VARIABLES
constexpr int GAME_SPEED = {1200};

constexpr std::size_t BOARD_WIDTH{ 12 };
constexpr std::size_t BOARD_HEIGHT{ 21 };
constexpr std::size_t BOARD_START_POSX{ 300 };
constexpr std::size_t BOARD_START_POSY{ 30 };

constexpr std::size_t BLOCK_SIZE{ 30 };
constexpr std::size_t BLOCK_OFFSET{ 3 };

constexpr std::size_t TETRAMINO_START_X{ 0 };
constexpr std::size_t TETRAMINO_START_Y{ BOARD_WIDTH / 2 };


constexpr float HELD_TET_X{ 100 };
constexpr float HELD_TET_Y{ 110 };

constexpr float NEXT_TET_X{ 800 };
constexpr float NEXT_TET_Y{ 100 };



