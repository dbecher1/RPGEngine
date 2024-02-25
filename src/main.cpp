
#include "game/ResourceManager.h"
#include "game/SceneManager.h"
#include "game/gfx/Camera.h"
#include "tools/Timer.h"

// TODO: make these a struct for neatness

SDL_Window* window;
SceneManager* sceneManager;

static Timer timer;
double accumulator = 0.0;

int main() {

    if (SDL_Init(INIT_FLAGS) != 0) {
        ERROR_QUIT;
    }

    if (IMG_Init(IMG_FLAGS) != IMG_FLAGS) {
        ERROR_QUIT;
    }

    //window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INITIAL_WIDTH, INITIAL_HEIGHT, WINDOW_FLAGS);
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
    if (window == nullptr) {
        ERROR_QUIT;
    }

    InputManager::Init();
    sceneManager = new SceneManager(window);

    bool quit = false;
    SDL_Event e;
    bool first_loop = true;

    do {
        // Process events
        timer.tick();

        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (e.window.event) {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                        case SDL_WINDOWEVENT_RESIZED:
                            sceneManager->windowResizeEvent(e.window.data1, e.window.data2);
                            break;
                        default: break;
                    }
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.scancode == SDL_SCANCODE_5) {
                        std::cout << "Manual resize to default!" << std::endl;
                        sceneManager->resetDefaultWindowSize(window);
                    }
                default: break;
            }
        }
        InputManager::Poll();

        if (first_loop) {
            first_loop = false;
            timer.dt = 0;
        }

        sceneManager->Update(timer.dt);

        accumulator += timer.dt;

        while (accumulator >= FIXED_UPDATE_INTERVAL) {
            sceneManager->FixedUpdate();
            accumulator -= FIXED_UPDATE_INTERVAL;
        }
        sceneManager->Draw();
    } while (!quit);

    delete sceneManager;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
