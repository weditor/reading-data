//
// Created by viruser on 2022/12/28.
//

#ifndef UNTITLED_ERRORS_H
#define UNTITLED_ERRORS_H

#include <exception>
#include <string>
#include <utility>

struct JsonParseError: public std::exception {
    explicit JsonParseError(std::string reason): reason(std::move(reason)){};
    [[nodiscard]] const char * what() const noexcept override {
        return this->reason.c_str();
    }
    std::string reason;
};

struct IllegalStateError: public std::exception {
    explicit IllegalStateError(std::string reason=""): reason(std::move(reason)){};
    [[nodiscard]] const char * what() const noexcept override {
        return this->reason.c_str();
    }
    std::string reason;
};

#endif //UNTITLED_ERRORS_H
