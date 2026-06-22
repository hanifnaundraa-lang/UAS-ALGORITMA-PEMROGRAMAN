#pragma once
#include <stdexcept>
#include <string>

/*==================================================
  MATERI: EXCEPTION HANDLING
==================================================*/

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message)
        : std::runtime_error("[Game Error] " + message) {}
};

class FileException : public std::runtime_error {
public:
    explicit FileException(const std::string& message)
        : std::runtime_error("[File Error] " + message) {}
};

class InputException : public std::runtime_error {
public:
    explicit InputException(const std::string& message)
        : std::runtime_error("[Input Error] " + message) {}
};

class InsufficientFundsException : public std::runtime_error {
public:
    explicit InsufficientFundsException(const std::string& message)
        : std::runtime_error("[Funds Error] " + message) {}
};
