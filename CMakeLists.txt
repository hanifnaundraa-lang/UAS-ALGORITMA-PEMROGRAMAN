cmake_minimum_required(VERSION 3.16)

project(AstroStrike VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/player.cpp
    src/bullet.cpp
    src/enemy.cpp
    src/scoremanager.cpp
    src/inventory.cpp
    src/shop.cpp
    src/trading.cpp
    src/gacha.cpp
)

set(HEADERS
    src/game.h
    src/player.h
    src/bullet.h
    src/enemy.h
    src/scoremanager.h
    src/inventory.h
    src/shop.h
    src/trading.h
    src/gacha.h
    src/gamedata.h
    src/gameconfig.h
    src/gameutils.h
    src/gameexception.h
)

# Build terminal executable (no Qt dependency)
add_executable(AstroStrike ${SOURCES} ${HEADERS})

# Include src directory for header resolution
target_include_directories(AstroStrike PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Set working directory so data/ is accessible at runtime
set_target_properties(AstroStrike PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
