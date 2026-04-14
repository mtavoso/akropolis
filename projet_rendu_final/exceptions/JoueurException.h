#pragma once
#include <stdexcept>
#include <string>

class JoueurException : public std::runtime_error {
public:
    explicit JoueurException(const std::string& message)
        : std::runtime_error(message) {}
};

