#include "path.hpp"

#include <vector>
#include <iostream>
#include <string>
#include <stdexcept>

Path::Path(size_t length, size_t starting_row) {

    if (length < 0 || starting_row < 0) {
        throw std::runtime_error("length and or row out of bounds on path");
    }
    length_ = length;
    starting_row_ = starting_row;

    path_ = std::vector<size_t>(length_);
    path_[0] = starting_row;
}

void Path::SetLoc(size_t col, size_t row) {
    if (col >= length_) {
        throw std::runtime_error("setting location out of bounds");
    }

    path_.at(col) = row;
}

