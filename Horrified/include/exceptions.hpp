#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <string>

class FileOpenningExecption : public std::exception {
private:
    std::string message;
public:
    explicit FileOpenningExecption(const std::string& msg = "Bad address exception");
    const char* what() const noexcept override;
};

#endif // EXCEPTIONS_HPP
