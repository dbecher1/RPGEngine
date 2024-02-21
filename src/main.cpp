
#include "main.h"
#include "game/ResourceManager.h"
#include "game/SceneManager.h"
#include "game/SpriteBatch.h"
#include "tools/Timer.h"
#include <cmath>

SDL_Window* window;
SDL_Renderer* renderer;
ResourceManager* resourceManager;
SceneManager* sceneManager;
SpriteBatch* spriteBatch;

const int GOAL_FPS = 60;
const float CYCLE_TIME = 1.0f / GOAL_FPS;
static Timer sys_timer;
float accumulator = 0.0f;

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

    Environment* env = resourceManager->getEnvironment("Field");
    Maps* maps = resourceManager->getMap("demo");
    maps->Dump();

    do {
        // Process events
        // TODO: Input handler/mapper

        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (e.window.event) {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                        case SDL_WINDOWEVENT_RESIZED:
                            spriteBatch->windowResizeEvent(e.window.data1, e.window.data2);
                            break;
                        default: break;
                    }
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.scancode == SDL_SCANCODE_5) {
                        std::cout << "Manual resize to default!" << std::endl;
                        spriteBatch->resetDefaultWindowSize(window);
                    }
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
            // TODO: fix environment draw
            /*
            DrawCommand envDraw;
            env->Draw(&envDraw);
            spriteBatch->Add(envDraw);
             */
            maps->Draw(spriteBatch);

            spriteBatch->Draw();
        }

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