#ifndef GAMEUTILS_H
#define GAMEUTILS_H

#include <random>

// Material: Inline Function, Default Argument, Function Template
namespace GameUtils {
    // Material: Inline Function
    inline float clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    // Material: Function Template
    template<typename T>
    T getRandomNumber(T min, T max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(static_cast<float>(min), static_cast<float>(max));
        return static_cast<T>(dis(gen));
    }

    // Material: Default Argument
    inline float calculateDistance(float x1, float y1, float x2 = 0.0f, float y2 = 0.0f) {
        return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    }
}

#endif // GAMEUTILS_H
