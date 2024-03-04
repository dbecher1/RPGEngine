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
        src/game/scenes/OverworldScene.cpp
        src/tools/ImagePacker.cpp
        src/game/gfx/Environment.cpp
        src/game/SpriteBatch.cpp
        src/game/gfx/animations/Animation.cpp
        src/game/gfx/animations/AnimationStateMachine.cpp
        src/game/InputManager.cpp
        src/tools/Timer.cpp
        src/game/world/Maps.cpp
        src/game/gfx/Camera.cpp
        src/game/ui/UIManager.cpp
        src/game/ui/UIElement.cpp
        src/game/state/GlobalState.cpp
        src/game/entities/GlobalEntity.cpp
        src/game/entities/BattleEntity.cpp
        src/game/entities/OverworldEntity.cpp
        src/game/EntityManager.cpp
        src/game/events/EventPoller.cpp
        src/game/SpriteBatch.h
        src/game/SceneManager.h
        src/game/ResourceManager.h
        src/game/InputManager.h
        src/game/EntityManager.h
        src/game/world/Maps.h
        src/game/ui/UIManager.h
        src/game/ui/UIElement.h
        src/game/state/GlobalState.h
        src/game/scenes/Scene.h
        src/game/scenes/OverworldScene.h
        src/game/misc/macros.h
        src/game/gfx/Environment.h
        src/game/gfx/DrawCommand.h
        src/game/gfx/Camera.h
        src/game/events/Event.h
        src/game/events/EventPoller.h
        src/game/entities/BattleEntity.h
        src/game/entities/GlobalEntity.h
        src/game/entities/IBaseEntity.h
        src/game/entities/OverworldEntity.h
        src/game/battle/Attributes.h
        src/game/battle/ElementalAffinity.h
        src/game/battle/Job.h
        src/math/MiscMath.h
        src/math/Vector2.h
        src/tools/ImagePacker.h
        src/tools/MapLoader.hpp
        src/tools/Tile.h
        src/tools/Timer.h
        src/game/ui/Font.cpp
        src/game/ui/Font.h
        src/tools/extern/schrift.c
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
