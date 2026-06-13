#ifndef GAMECONFIG_H
#define GAMECONFIG_H

// Material: Namespace
namespace GameConfig {
    inline constexpr int WINDOW_WIDTH = 800;
    inline constexpr int WINDOW_HEIGHT = 600;
    
    inline constexpr float PLAYER_SPEED = 5.0f;
    inline constexpr int INITIAL_LIVES = 3;
    
    inline constexpr float BULLET_SPEED = 10.0f;
    inline constexpr int ENEMY_SPAWN_INTERVAL = 2000; // ms
}

#endif // GAMECONFIG_H
