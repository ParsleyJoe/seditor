#include "editor.hpp"

#include "gap_buffer.hpp"

// File Opening and saving
#include <nfd.h>
#include <filesystem>
#include <fstream>


// ## HOLY FRICK DUDE 
// CURSOR MOVEMENT SUX

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

	// Native file dialog Init
	NFD_Init();

	// SDL Text Input
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
		switch (event->key.key)
		{
		case SDLK_RETURN:
			buf.insertChar(cursorPos, '\n');
			cursorPos++;
		break;
		case SDLK_BACKSPACE:
			if (cursorPos <= 0)
			{
				cursorPos = 0;
			}
			else
			{
				buf.removeChar(cursorPos);
				cursorPos--;
			}
			break;

		default:
			break;
		}

		// Shortcuts
		if (event->key.mod & SDL_KMOD_LCTRL)
		{
			switch (event->key.key)
			{
			case SDLK_S:
				// --------------------------
				// Save
				if (m_file == "") // file opened for the first time
				{
					m_file = openFile();
				}

				if (m_file == "") // user clicked cancel in openFile()
				{
					break;
				}
				else
				{
					std::fstream file(m_file.c_str(), std::ios::out | std::ios::trunc);
					if (!file.is_open())
					{
						SDL_Log("ERORR::CTRL+S CANNOT SAVE\n");
					}

					std::string str;
					str += buf.getLeftString();
					str += buf.getRightString();
					file << str;

					SDL_Log("Saved Succesfully\n");
				}
				break;
			case SDLK_V:
			{
				char* str = SDL_GetClipboardText();
				std::string clipboardTxt(str);
				SDL_free(str);
				for (int i = 0; i < clipboardTxt.size(); i++)
				{
					buf.insertChar(cursorPos, clipboardTxt[i]);
					cursorPos++;
				}
			}
				break;
			default:
				break;
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
		/*if (surface == NULL)
		{
			SDL_Log("Error::Editor::Render() surface is NULL, %s", SDL_GetError());
		}*/

		SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
		/*if (texture == NULL)
		{
			SDL_Log("Error::Editor::Render() texture is NULL, %s", SDL_GetError());
		}*/

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

	SDL_RenderPresent(m_renderer);

	return SDL_APP_CONTINUE;
}

void Editor::MoveCursor(SDL_Event* event)
{
	std::string str{};
	str += buf.getLeftString();
	str += buf.getRightString();
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
		int lineEnd = findNextNewLine(str, cursorPos);

		int lineStart = findPreviousNewLine(str, cursorPos);
		int pos = cursorPos - lineStart;
		cursorPos -= ((pos < 0) ? 0 : pos);
		cursorPos--;

		if (cursorPos < 0)
			cursorPos = 0;

	}
	break;
	case SDLK_DOWN:
	{
		int lineEnd = findNextNewLine(str, cursorPos);

		int pos = lineEnd - cursorPos;
		cursorPos += ((pos < 0) ? 0 : pos); // now at the line end
		cursorPos++; // now at next line

		if (cursorPos > str.size())
		{
			cursorPos = str.size();
		}
	}
	break;
	default:
		break;
	}
}

int findNextNewLine(std::string str, int cursorPos)
{
	int index = cursorPos - 1;
	if (cursorPos == 0)
		index = 0;

	while (index < str.size() && str[index] != '\n')
	{
		index++;
	}

	return index;
}

int findPreviousNewLine(std::string str, int cursorPos)
{
	int index = cursorPos - 1;
	if (cursorPos == 0)
		index = 0;

	while (index > 0 && str[index] != '\n')
	{
		index--;
	}

	return index;
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


std::string openFile()
{
	std::string resultPath{ "" };
	nfdu8char_t* outpath;
	nfdu8filteritem_t filters[1] = { {"TextFiles", "txt"}};
	nfdopendialogu8args_t args = { 0 };
	args.filterList = filters;
	args.filterCount = 1;
	nfdresult_t result = NFD_OpenDialogU8_With(&outpath, &args);
	if (result == NFD_OKAY)
	{
		resultPath = outpath;
		NFD_FreePathU8(outpath);
	}
	else if (result == NFD_CANCEL)
	{
		resultPath = "";
	}
	else
	{
		SDL_Log("ERROR::editor.cpp::openFile(), Cannot open file%s\n", NFD_GetError());
	}

	return resultPath;
}