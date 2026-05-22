#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <string>
#include <vector>

struct Cursor
{
	int x, y;
};

class Editor
{
public:
	SDL_Window* m_window = NULL;
	SDL_Renderer* m_renderer = NULL;
	std::vector<std::string> m_lines{};
	TTF_Font* m_font = NULL;
	int m_currentLine = 0;
	SDL_Surface* m_surface = NULL;
	SDL_Texture* m_texture = NULL;
	SDL_Color textColor = {255, 255, 255, 255};


	SDL_AppResult Init();
	SDL_AppResult HandleEvents(SDL_Event* event);
	SDL_AppResult Render();
};