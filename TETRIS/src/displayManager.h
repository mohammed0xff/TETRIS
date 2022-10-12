#pragma once

#include <vector>
#include <deque>
#include <list>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include "shapes.h"
#include "board.h"


class DisplayManager
{

public:
	DisplayManager();
	~DisplayManager();	

	bool init();
	bool loadGameAssets();
	
	void drawBoard(const Board&);
	void drawTetramino(const Tetramino* tetramino, Uint8 alpha, int xBoard = -1, int yBoard = -1);
	void drawTetraminoOffBoard(const Tetramino* tetramino, Uint8 alpha, float x, float y);
	void drawShadow(const Tetramino* tetramino, const Board& m_board);
	void drawNext(const std::list<Tetramino*> tets);
	void drawHeld(const Tetramino*);
	void drawText(std::string text, int x, int y, int w, int h);
	void drawUpdate();
	void clear();

	SDL_Texture* loadTexture(std::string path);

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Rect rect;
	TTF_Font* font;
	SDL_Color textColor;
	SDL_Surface* textSurface;
	SDL_Texture* textTexture;
	SDL_Texture* bg_texture;
	std::vector<SDL_Texture*> m_ShapesTextures;
};