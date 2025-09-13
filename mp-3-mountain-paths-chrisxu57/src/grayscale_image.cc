#include "grayscale_image.hpp"
#include "color.hpp"
#include "elevation_dataset.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cmath>

GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset) {
    height_ = dataset.Height();
    width_ = dataset.Width();
    image_ = std::vector<std::vector<Color>>(height_, std::vector<Color>(width_));

    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            int shade_of_gray = 0;
            if (!(dataset.MaxEle() == dataset.MinEle())) {
                auto datum_at = static_cast<double>(dataset.DatumAt(i, j));
                auto max = static_cast<double>(dataset.MaxEle());
                auto min = static_cast<double>(dataset.MinEle());
                shade_of_gray = static_cast<int>(std::round(((datum_at - min) / (max - min)) * kMaxColorValue));
            }

            Color color(shade_of_gray, shade_of_gray, shade_of_gray);
            image_[i][j] = color;
        }
    }
}

GrayscaleImage::GrayscaleImage(const std::string& filename, size_t width, size_t height) {
    height_ = height;
    width_ = width;

    ElevationDataset dataset(filename, width, height);
    image_ = std::vector<std::vector<Color>>(height_, std::vector<Color>(width_));

    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < width_; j++) {
            int shade_of_gray = 0;
            if (!(dataset.MaxEle() == dataset.MinEle())) {
                auto datum_at = static_cast<double>(dataset.DatumAt(i, j));
                auto max = static_cast<double>(dataset.MaxEle());
                auto min = static_cast<double>(dataset.MinEle());
                shade_of_gray = static_cast<int>(std::round(((datum_at - min) / (max - min)) * kMaxColorValue));
            }
            Color color(shade_of_gray, shade_of_gray, shade_of_gray);
            image_[i][j] = color;
        }
    }
}

const Color& GrayscaleImage::ColorAt(int row, int col) const {
    return image_[row][col];
}

void GrayscaleImage::ToPpm(const std::string& name) {
    std::ofstream file(name);

    if (!file) {
        throw std::runtime_error("Cannot open file");
    }

    file << "P3\n";
    file << width_ << " " << height_ << "\n";
    file << kMaxColorValue << "\n";

    for (size_t row = 0; row < height_; row++) {
        for (size_t col = 0; col < width_; col++) {
            const Color& color = image_[row][col];
            file << color.Red() << " " << color.Green() << " " << color.Blue() << " ";
        }
        file << "\n";
    }

    file.close();
}