cmake_minimum_required(VERSION 3.26)
project(RPG)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE DEBUG)
#set(CMAKE_BUILD_TYPE RELEASE)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
#find_package(SDL_mixer REQUIRED)

set (SDL2_IMAGE_LIBRARIES /Library/Frameworks/SDL2_image.framework/SDL2_image)

if (SDL2_FOUND)
    message("SDL2 include directory found: ${SDL2_INCLUDE_DIR}")
    message("SDL2 library found: ${SDL2_LIBRARIES}")
endif ()

if (SDL2_IMAGE_FOUND)
    message("SDL2 image include directory found: ${SDL_IMAGE_INCLUDE_DIR}")
    message("SDL2 image library found: ${SDL2_IMAGE_LIBRARIES}")
endif ()

if (SDL_MIXER_FOUND)
    message("SDL2 mixer include directory found: ${SDL_MIXER_INCLUDE_DIR}")
    message("SDL2 mixer library found: ${SDL_MIXER_LIBRARIES}")
endif ()

include_directories(${SDL2_INCLUDE_DIR} ${SDL_IMAGE_INCLUDE_DIR})

add_executable(RPG
        src/main.cpp
        src/game/ResourceManager.cpp
        src/game/SceneManager.cpp
        src/game/scenes/Scene.cpp
        src/tools/ImagePacker.cpp
        src/game/gfx/Environment.cpp
        src/math/Vector2.cpp
        src/game/SpriteBatch.cpp
        src/game/SpriteBatch.h
        src/game/gfx/animations/Animation.cpp
        src/game/gfx/animations/AnimationStateMachine.cpp
        src/game/gfx/animations/AnimationStateMachine.h
        src/game/entities/Entity.cpp
        src/game/InputManager.cpp
        src/game/InputManager.h
        src/tools/Timer.cpp
        src/tools/Timer.h
        src/math/MiscMath.h
        src/tools/MapLoader.cpp
        src/game/world/Maps.cpp
        src/game/world/Maps.h
        src/tools/Tile.h
        src/game/gfx/Camera.cpp
        src/game/gfx/Camera.h
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
