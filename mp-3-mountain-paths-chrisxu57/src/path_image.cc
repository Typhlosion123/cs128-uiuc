#include "path_image.hpp"
#include "grayscale_image.hpp"
#include "path.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cmath>

PathImage::PathImage(const GrayscaleImage& image, const ElevationDataset& dataset) {
    height_ = image.Height();
    width_ = image.Width();
    path_image_ = image.GetImage();

    const int kRed = 252;
    const int kGreen = 25;
    const int kBlue = 63;

    paths_ = std::vector<Path>(height_, Path(width_, 0));

    for (size_t i = 0; i < height_; i++) {
        Path path(width_, i);
        path_image_[i][0] = Color(kRed, kGreen, kBlue);
        for (size_t col = 0; col < width_ - 1; col++) {
            std::vector<size_t> current_path = path.GetPath();
            unsigned int current_row = current_path[col];
            int current_value = dataset.DatumAt(current_path[col], col);
            unsigned int best_row = current_row;
            int best_diff = abs(dataset.DatumAt(current_row, col + 1) - current_value);

            if (current_row < height_ - 1 && abs(dataset.DatumAt(current_row + 1, col + 1) - current_value) < best_diff) {
                best_diff = abs(dataset.DatumAt(current_row + 1, col + 1) - current_value);
                best_row = current_row + 1;
            }

            if (current_row > 0 && abs(dataset.DatumAt(current_row - 1, col + 1) - current_value) < best_diff) {
                best_diff = abs(dataset.DatumAt(current_row - 1, col + 1) - current_value);
                best_row = current_row - 1;
            }

            path.IncEleChange(best_diff);
            path.SetLoc(col + 1, best_row);
            path_image_[best_row][col + 1] = Color(kRed, kGreen, kBlue);
        }
        paths_.at(i) = path;
    }
}

void PathImage::ToPpm(const std::string& name) {
    std::ofstream file(name);

    const int kRed = 31;
    const int kGreen = 253;
    const int kBlue = 13; 

    unsigned int best_diff = paths_[0].EleChange();
    Path best_path = paths_[0];

    for (const Path& path : paths_) {
        if (path.EleChange() < best_diff) {
            best_diff = path.EleChange();
            best_path = path;
        }
    }
    for (size_t i = 0; i < width_; i++) {
        size_t best_row = best_path.GetPath()[i];
        path_image_[best_row][i] = Color(kRed, kGreen, kBlue);
    }

    file << "P3\n";
    file << width_ << " " << height_ << "\n";
    file << kMaxColorValue << "\n";

    for (size_t row = 0; row < height_; row++) {
        for (size_t col = 0; col < width_; col++) {
            const Color& color = path_image_[row][col];
            file << color.Red() << " " << color.Green() << " " << color.Blue() << " ";
        }
        file << "\n";
    }

    file.close();




}