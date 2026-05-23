#include "editor.hpp"

#include "gap_buffer.hpp"

SDL_AppResult Editor::Init()
{
	// Testing gap_buffer
	/*buf.insertChar(0, 'c');
	buf.insertChar(1, 'b');
	buf.insertChar(2, 'd');
	buf.insertChar(3, 'e');
	buf.insertChar(4, 'z');
	buf.insertChar(3, 'L');*/
	//buf.checkGapSize(30);

	// Create window
	if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE, &m_window, &m_renderer)) {
		SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	SDL_Log("Left String: %s, Right String: %s", buf.getLeftString().c_str(), buf.getRightString().c_str());

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

	// The Key
	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		if (event->key.key == SDLK_RETURN)
		{
			// Add enter functionality (probably just add a \n)
			buf.insertChar(cursorPos.col, '\n');
			cursorPos.col++;
		}
		if (event->key.key == SDLK_BACKSPACE)
		{
			// Yeah this is non existant as well
			buf.insertChar(3, 'z');
			cursorPos.col++;
		}
	}

	// The Alphabet on the key
	if (event->type == SDL_EVENT_TEXT_INPUT)
	{
		buf.insertChar(cursorPos.col, *event->text.text);
		cursorPos.col++;
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
	std::string str{};
	str += buf.getLeftString();
	str += buf.getRightString();
	
	// no. of \n
	int i = 0;
	int lastNewLine = 0;
	bool done = false;
	while(!done)
	{
		std::string s{};
		if (str.size() <= 0)
		{
			done = true;
		}
		int newLine = str.find('\n', lastNewLine + 1);
		if (newLine == std::string::npos) // doesnt exist after lastNewLine
		{
			done = true;
			if (lastNewLine == 0) // never existed 
				s = str;
			else
				s = str.substr(lastNewLine, str.size() - lastNewLine); // 2nd arguement is count instead of pos
		}
		else
		{
			s = str.substr(lastNewLine, newLine - 1);
			lastNewLine = newLine;
		}

		SDL_Surface* surface = TTF_RenderText_Solid(m_font, s.c_str(), s.size(), textColor);
		if (surface == NULL)
		{
			SDL_Log("Error::Editor::Render() surface is NULL, %s", SDL_GetError());
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
		if (texture == NULL)
		{
			SDL_Log("Error::Editor::Render() texture is NULL, %s", SDL_GetError());
		}

		SDL_DestroySurface(surface);

		float w, h;
		SDL_GetTextureSize(texture, &w, &h);

		SDL_FRect rect = { 0, y + (i * h), w, h };
		SDL_RenderTexture(m_renderer, texture, NULL, &rect);

		SDL_DestroyTexture(texture);

		i++;
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