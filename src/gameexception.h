#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <exception>
#include <string>

// Material: Exception Handling
class GameException : public std::exception {
public:
    explicit GameException(const std::string& message) : m_message(message) {}
    const char* what() const noexcept override {
        return m_message.c_str();
    }
private:
    std::string m_message;
};

class FileLoadException : public GameException {
public:
    explicit FileLoadException(const std::string& filename) 
        : GameException("Failed to load file: " + filename) {}
};

#endif // GAMEEXCEPTION_H
