#include "exceptions.hpp"

FileOpenningExecption::FileOpenningExecption(const std::string& msg)
    : message(msg) {}

const char* FileOpenningExecption::what() const noexcept {
    return message.c_str();
}
