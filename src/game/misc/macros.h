//
// Created by Daniel Becher on 2/13/24.
//

#ifndef RPG_MACROS_H
#define RPG_MACROS_H

#include <iostream>
#include "SDL.h"
#include "SDL2_image/SDL_image.h"

// Initial window settings
#define TITLE "Test"
#define INITIAL_WIDTH 720
#define INITIAL_HEIGHT 480
#define ASPECT_RATIO 1.5

// Flags
#define IMG_FLAGS IMG_INIT_PNG
#define INIT_FLAGS (SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO | SDL_INIT_TIMER)
#define WINDOW_FLAGS SDL_WINDOW_RESIZABLE
#define RENDERER_FLAGS 0
#define PIXEL_FORMAT SDL_PIXELFORMAT_ABGR8888

// Other useful macros/misc
#define ERROR_QUIT std::cerr << SDL_GetError() << std::endl; return false;
#define EPSILON 0.00001f

// Flag to indicate if the filename will include the file extension
// Probably will always keep this false
#define INCLUDE_EXTENSION false
#ifdef NDEBUG
#define DEBUG_PRINT false
#define SAVE_IMG false
#else
// make this true/false depending on if we want to print
// I find this easier than combing through the function to find the print statement
#define DEBUG_PRINT false
// flag to save the packed image, not going to be done in production
#define SAVE_IMG true
#endif

#endif //RPG_MACROS_H
