#include "editor.hpp"

#include "gap_buffer.hpp"

// ###########
// !!!!TODO:
// CODE FULL OF BUGS,
// MOVING CURSOR.


SDL_AppResult Editor::Init()
{
#ifdef DEBUG
	_CrtSetDbgFlag(
		_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF
	)
#endif // DEBUG | to check heap corruption

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

	m_font = TTF_OpenFont(RESOURCES_PATH"fonts/CascadiaMono-Regular.ttf", 16);
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
		MoveCursor(event);
		if (event->key.key == SDLK_RETURN)
		{
			// Add enter functionality (probably just add a \n)
			buf.insertChar(cursorPos, '\n');
			cursorPos++;
		}
		if (event->key.key == SDLK_BACKSPACE)
		{
			// Yeah this is non existant as well
			//buf.insertChar(3, 'z');
			//cursorPos++;
			if (cursorPos <= 0)
			{
				cursorPos = 0;
			}
			else
			{
				buf.removeChar(cursorPos);
				cursorPos--;
			}
		}
		
	}

	// The Alphabet on the key
	if (event->type == SDL_EVENT_TEXT_INPUT)
	{
		buf.insertChar(cursorPos, *event->text.text);
		cursorPos++;
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
	

	// Render Text
	SDL_SetRenderDrawColor(m_renderer, 31, 31, 31, 255); // background color
	SDL_RenderClear(m_renderer);

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
				s = str.substr(lastNewLine + 1, str.size() - lastNewLine); // 2nd arguement is count instead of pos
		}
		else
		{
			int startIndex = (lastNewLine == 0) ? 0 : (lastNewLine + 1);
			int count = newLine - startIndex;
			s = str.substr(startIndex, count); // also 2nd arguement is count
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

		SDL_FRect rect = { 10, (i * h), w, h };
		SDL_RenderTexture(m_renderer, texture, NULL, &rect);

		SDL_DestroyTexture(texture);

		i++;
	}

	// Render Cursor
	int w, h;
	TTF_GetStringSize(m_font, "M", 0, &w, &h);
	// col
	int lineStart = findLineStart(buf, cursorPos);
	int col = cursorPos - lineStart;
	// row
	int count = 0;
	for (int i = cursorPos - 1; i > 0; i--)
	{
		if (str[i] == '\n')
			count++;
	}


	SDL_FRect cursorRect = { 10 + (col * w),  (count * h), w, h };
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 80);
	SDL_RenderFillRect(m_renderer, &cursorRect);

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

void Editor::MoveCursor(SDL_Event* event)
{
	switch (event->key.key)
	{
	case SDLK_LEFT:
		cursorPos--;
		if (cursorPos < 0)
			cursorPos = 0;
		break;
	case SDLK_RIGHT:
	{
		cursorPos++;
		std::string str;
		str += buf.getLeftString();
		str += buf.getRightString();
		if (cursorPos > str.size())
			cursorPos--;
	}
		break;
	case SDLK_UP:
	{
		// Find Line Start
		int lineStart = findLineStart(buf, cursorPos);
		if (lineStart == 0)
		{
			break;
		}
		// current column
		int column = cursorPos - lineStart;
		prefferedColumn = column;
		// previous Line start
		int prevLineStart = findLineStart(buf, --lineStart);
		int lineLength = (lineStart - 1) - prevLineStart;
		int targetColumn = std::min(prefferedColumn, lineLength);

		cursorPos = prevLineStart + targetColumn;
	}
	break;
	case SDLK_DOWN:
	{
		// line start
		int lineStart = findLineStart(buf, cursorPos);
		// current Column
		int column = cursorPos - lineStart;
		prefferedColumn = column;
		// find next line start
		int lineEnd = findLineEnd(buf, cursorPos);
		std::string str{};
		str += buf.getLeftString();
		str += buf.getRightString();
		if (lineEnd > str.size())
			break;
		int nextLineStart = findLineStart(buf, (lineEnd + 2));
		int lineLength = findLineEnd(buf, nextLineStart) - (nextLineStart - 1);
		int targetColumn = std::min(prefferedColumn, lineLength - 1);

		cursorPos = nextLineStart + targetColumn;
	}
	break;
	default:
		break;
	}
}

void Editor::MoveCursorUp()
{

}

int findLineStart(GapBuffer& buf, int cursorPos)
{
	std::string str{};
	str += buf.getLeftString();
	str += buf.getRightString();

	if (cursorPos >= str.size())
	{
		cursorPos = str.size() - 1;
	}
	int lineStart = cursorPos;
	while (lineStart > 0 && str[lineStart - 1] != '\n')
	{
		lineStart--;
	}

	return lineStart;
}

int findLineEnd(GapBuffer& buf, int cursorPos)
{
	std::string str{};
	str += buf.getLeftString();
	str += buf.getRightString();

	if (cursorPos >= str.size())
	{
		cursorPos = str.size() - 1;
		return cursorPos; // must already be at line end
	}

	int lineEnd = cursorPos;
	while (lineEnd < str.size() && str[lineEnd + 1] != '\n')
	{
		lineEnd++;
	}

	return lineEnd;
}