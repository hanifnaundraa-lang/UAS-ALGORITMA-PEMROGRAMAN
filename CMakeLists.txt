cmake_minimum_required(VERSION 3.16)

project(AstroStrike VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Qml Quick)

qt_standard_project_setup()

qt_add_executable(AstroStrike
    src/main.cpp
    src/gamecontroller.h src/gamecontroller.cpp
    src/player.h src/player.cpp
    src/enemy.h src/enemy.cpp
    src/bullet.h src/bullet.cpp
    src/scoremanager.h src/scoremanager.cpp
    src/gamedata.h
    src/gameconfig.h
    src/gameutils.h
    src/gameexception.h
)

qt_add_qml_module(AstroStrike
    URI AstroStrike
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/MainMenu.qml
        qml/GameScreen.qml
        qml/GameOverScreen.qml
        qml/LeaderboardScreen.qml
    RESOURCES
        assets/player.png
        assets/enemy.png
        assets/bullet.png
        assets/background.png
)

target_link_libraries(AstroStrike PRIVATE Qt6::Gui Qt6::Qml Qt6::Quick)

install(TARGETS AstroStrike
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
