#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <string>
#include <vector>

#include "editor.hpp"

static Editor editor;

// This function runs once at startup. so like void Start()
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
	return editor.Init();;
}

// This function runs when a new event (mouse input, keypresses, etc) occurs
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{

	return editor.HandleEvents(event);
}

// This Is the main function, runs every frame
SDL_AppResult SDL_AppIterate(void* appstate)
{
	return editor.Render();
}

// This function runs once at shutdown
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{

}