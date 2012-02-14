include_directories(
    ${OGRE_INCLUDE_DIR}
    ${OIS_INCLUDE_DIR}
    ${ReQuest_BINARY_DIR}
)

# The code that realizes the game itself. All the game dependent code is found here
add_executable (ReQuest WIN32 
    ReQuest.cpp
    Prerequisites.h
    Game.cpp 
    Game.h
    ConfigStorage.h
    ConfigStorage.cpp
    GameState.h
    GameState.cpp
    Exception.h
    Exception.cpp
    MenuState.h
    MenuState.cpp
    PlayState.h
    PlayState.cpp
    ObjectTypes.h
    PlayerObject.h
    PlayerObject.cpp
    BulletObject.h
    BulletObject.cpp
    MineObject.h
    MineObject.cpp
    EnemyObject.h
    EnemyObject.cpp
    Starfield.h
    Starfield.cpp
    Constants.h
    Constants.cpp
    GameEffects.h
    GameEffects.cpp
)

# Game independent subsystems. No game dependent code here
add_library(SubSystems
    SubSystem.h
    InputSystem.h
    InputSystem.cpp
    RendererSystem.h
    RendererSystem.cpp
    MenuSystem.h
    MenuSystem.cpp
    ObjectSystem.h
    ObjectSystem.cpp
    EffectSystem.h
    EffectSystem.cpp
)

target_link_libraries(ReQuest
    ${OGRE_LIBRARIES}
    ${OIS_LIBRARIES}
    SubSystems
)
