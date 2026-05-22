#include "editor.hpp"

SDL_AppResult Editor::Init()
{
	// Create window
	if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE, &m_window, &m_renderer)) {
		SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (TTF_Init() == -1)
	{
		SDL_Log("TTF_Init: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	m_font = TTF_OpenFont(RESOURCES_PATH"fonts/TitilliumWeb-SemiBold.ttf", 16);
	if (!m_font)
	{
		SDL_Log("TTF_OpenFont: %s\n", SDL_GetError());
		// handle error
	}

	SDL_Rect area = { 0, 0, 800,600 };
	SDL_SetTextInputArea(m_window, &area, 0);
	if (!SDL_StartTextInput(m_window))
	{
		// Failed to start input
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult Editor::HandleEvents(SDL_Event* event)
{
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	// Add an empty string just in case its empty to avoid acces violations
	if (m_lines.size() < 1)
	{
		m_lines.push_back("");
	}

	// The Key
	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		if (event->key.key == SDLK_RETURN)
		{
			m_lines.push_back("");
			m_currentLine++;
		}
		if (event->key.key == SDLK_BACKSPACE)
		{
			if (!m_lines[m_currentLine].empty())
				m_lines[m_currentLine].pop_back();
		}
	}

	// The Alphabet on the key
	if (event->type == SDL_EVENT_TEXT_INPUT)
	{

		m_lines[m_currentLine] += event->text.text;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult Editor::Render()
{
	// Probably want to use this function at some point
	//SDL_SetRenderScale(m_renderer, scale, scale);

	//// Draw message
	//SDL_SetRenderDrawColor(m_renderer, 31, 31, 31, 255);
	//SDL_RenderClear(m_renderer);
	//SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	//SDL_RenderDebugText(m_renderer, x, y, message);
	
	SDL_SetRenderDrawColor(m_renderer, 31, 31, 31, 255);
	SDL_RenderClear(m_renderer);
	int y = 0;
	for (const auto& line : m_lines)
	{
		SDL_Surface* surface = TTF_RenderText_Solid(m_font, line.c_str(), line.size(), textColor);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);

		SDL_DestroySurface(surface);

		float w, h;
		SDL_GetTextureSize(texture, &w, &h);

		SDL_FRect rect = { 0, y, w, h };
		SDL_RenderTexture(m_renderer, texture, NULL, &rect);

		SDL_DestroyTexture(texture);

		y += h;
	}
	if (m_texture != NULL)
	{
		/*float txtW, txtH;
		SDL_GetTextureSize(m_texture, &txtW, &txtH);
		SDL_FRect dstRect{ 0, 0, txtW, txtH };
		SDL_RenderTexture(m_renderer, m_texture, NULL, &dstRect);*/
	}
	SDL_RenderPresent(m_renderer);

	return SDL_APP_CONTINUE;
}