//
// Created by Daniel Becher on 2/13/24.
//

#ifndef RPG_MACROS_H
#define RPG_MACROS_H

#include <iostream>
#include <chrono>
#include "SDL.h"
#include "SDL2_image/SDL_image.h"

// Initial window settings
#define TITLE "Test"
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define INITIAL_WIDTH 720
#define INITIAL_HEIGHT 480
#define ASPECT_RATIO 1.5

// Flags

#define FORCE_GL false

#if FORCE_GL
#define GL SDL_WINDOW_OPENGL
#else
#define GL 0
#endif

#define IMG_FLAGS IMG_INIT_PNG
#define INIT_FLAGS (SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_VIDEO | SDL_INIT_TIMER)
#define WINDOW_FLAGS (SDL_WINDOW_RESIZABLE | GL)
#define RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
#define PIXEL_FORMAT SDL_PIXELFORMAT_ABGR8888
#define USE_SDL_TIMERS false
#define USE_THREADING false

// Other useful macros/misc
#define SDL_PRINT_ERR(msg) (std::cerr << msg << "\nError message: " << SDL_GetError() << std::endl)
#define ERROR_QUIT std::cerr << SDL_GetError() << std::endl; return 1;
#define EPSILON 0.00001f
#define FIXED_UPDATE_RATE 60.0
#define FIXED_UPDATE_INTERVAL (1.0 / FIXED_UPDATE_RATE)

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
