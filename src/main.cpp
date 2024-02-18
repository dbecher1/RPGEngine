
#include "main.h"
#include "game/ResourceManager.h"
#include "game/SceneManager.h"
#include "game/SpriteBatch.h"
#include "tools/Timer.h"
#include <cmath>
#include <string>

SDL_Window* window;
SDL_Renderer* renderer;
ResourceManager* resourceManager;
SceneManager* sceneManager;
SpriteBatch* spriteBatch;

const int GOAL_FPS = 60;
const float CYCLE_TIME = 1.0f / GOAL_FPS;
static Timer sys_timer;
float accumulator = 0.0f;

using hr_clock = std::chrono::high_resolution_clock;
using ms = std::chrono::duration<double, std::milli>;


int main() {

    if (!init())
        return 1;
    InputManager::Init();

    resourceManager = new ResourceManager();
    resourceManager->loadAllResources(renderer);

    spriteBatch = new SpriteBatch(renderer, resourceManager);

    sceneManager = new SceneManager();

    bool quit = false;
    SDL_Event e;
    SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);

    auto last = hr_clock::now();
    double t = 1 / 120.0;
    double dt;
    double fps = 0;
    int thresh = 256, counter = 0;

    do {
        // Process events
        // TODO: Input handler/mapper

        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                default: break;
            }
        }
        InputManager::Poll();
        sys_timer.tick();
        accumulator += sys_timer.elapsed_seconds;

        if (std::isgreater(accumulator, CYCLE_TIME)) {
            accumulator -= CYCLE_TIME; // ????

            static Timer physics_timer;
            physics_timer.tick();
            // std::cout << physics_timer.elapsed_seconds << std::endl;

            for (auto& e_ : *resourceManager->getEntities()) {
                e_.Update(physics_timer.elapsed_seconds);
            }
            for (auto& e_ : *resourceManager->getEntities()) {
                e_.Draw(spriteBatch);
            }

            spriteBatch->Draw();
        }
        double frame_time = 1000.0 / sys_timer.elapsed_seconds;
        counter++;
        if (counter > thresh) {
            counter = 0;
            fps /= thresh;
            std::string f = "FPS: " + std::to_string((int)fps);
            SDL_SetWindowTitle(window, f.c_str());
            fps = 0;
        }
        fps += frame_time;

        // Update TODO
        // stateMachine.Update(dt);
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));

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