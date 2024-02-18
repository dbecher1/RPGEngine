
#include "main.h"
#include "game/ResourceManager.h"
#include "game/SceneManager.h"
#include "game/SpriteBatch.h"
#include <cmath>
#include <string>

SDL_Window* window;
SDL_Renderer* renderer;
ResourceManager* resourceManager;
SceneManager* sceneManager;
SpriteBatch* spriteBatch;

using hr_clock = std::chrono::high_resolution_clock;
using ms = std::chrono::duration<double, std::milli>;

int main() {

    if (!init())
        return 1;

    resourceManager = new ResourceManager();
    resourceManager->loadAllResources(renderer);

    spriteBatch = new SpriteBatch(renderer, resourceManager);

    sceneManager = new SceneManager();

    bool quit = false;
    SDL_Event e;
    SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);

    auto last = hr_clock::now();
    double dt;
    double fps = 0;
    int thresh = 500, counter = 0;
    AnimationStateMachine stateMachine = resourceManager->asmLoader();
    int y = 0;

    do {
        auto current = hr_clock::now();
        dt = std::chrono::duration_cast<ms>(current - last).count();
        last = current;
        double frame_time = 1000.0 / dt;
        counter++;
        if (counter > thresh) {
            counter = 0;
            fps /= thresh;
            // std::cout << (int)fps << std::endl;
            std::string f = "FPS: " + std::to_string((int)fps);
            SDL_SetWindowTitle(window, f.c_str());
            fps = 0;
        }
        fps += frame_time;

        // Process events
        // TODO: Input handler/mapper

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        // Update TODO
        stateMachine.Update(dt);

        DrawCommand dc {
            .SpriteName = "cafe",
            .position = {20, 20},
            .dimensions = {200, 200},
            .z = 3,
            .useDimensions = true
        };
        // spriteBatch->Add(dc);

        DrawCommand dc2 {
                .SpriteName = "_Attack",
                .position = {20, 20},
                .z = 3,
        };
        stateMachine.Draw(&dc2);
        spriteBatch->Add(dc2);

        spriteBatch->Draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    } while (!quit);

    delete spriteBatch;
    delete sceneManager;
    delete resourceManager;
    destroy();

    return 0;
}

inline bool init() {
    if (SDL_Init(INIT_FLAGS) != 0) {
        ERROR_QUIT;
    }


    if (IMG_Init(IMG_FLAGS) != IMG_FLAGS) {
        ERROR_QUIT;
    }


    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, INITIAL_WIDTH, INITIAL_HEIGHT, WINDOW_FLAGS);
    if (window == nullptr) {
        ERROR_QUIT;
    }

    renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
    if (renderer == nullptr) {
        ERROR_QUIT;
    }
    return true;
}

inline void destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}