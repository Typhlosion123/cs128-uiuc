#include "elevation_dataset.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <limits>


ElevationDataset::ElevationDataset(const std::string& filename, size_t width, size_t height) {
    width_ = width;
    height_ = height;

    data_ = std::vector<std::vector<int>>(height_, std::vector<int>(width_));

    max_ele_ = std::numeric_limits<int>::lowest();
    min_ele_ = std::numeric_limits<int>::max();

    LoadData(filename);
}

int ElevationDataset::DatumAt(size_t row, size_t col) const {
    if (row >= height_ || col >= width_) {
        throw std::out_of_range("Index out of range");
    }

    return data_[row][col];
}

void ElevationDataset::LoadData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("cannot open file");
    }

    int value = 0;
    size_t count = 0;

    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            if (!(file >> value)) {
                throw std::runtime_error("cannot read data");
            }

            data_[i][j] = value;

            if (value > max_ele_) {
                max_ele_ = value;
            }
            if (value < min_ele_) {
                min_ele_ = value;
            }
            count++;
        }
    }

    if (file >> value) {
        throw std::runtime_error("too many values");
    }

    if (count != width_ * height_) {
        throw std::runtime_error("not enough data in file");
    }
}