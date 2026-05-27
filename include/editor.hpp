#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <string>
#include <vector>

#include "gap_buffer.hpp"

struct CursorPos
{
	int col = 0, row = 0;
	int bufPos = 0;
};

int findLineStart(GapBuffer& buf, int cursorPos);
int findLineEnd(GapBuffer& buf, int cursorPos);


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

	// Position relative to the buffer
	int cursorPos;
	int prefferedColumn;
	GapBuffer buf = GapBuffer(16);

	SDL_AppResult Init();
	SDL_AppResult HandleEvents(SDL_Event* event);
	SDL_AppResult Render();

	void MoveCursor(SDL_Event* event);
	void MoveCursorUp();

};