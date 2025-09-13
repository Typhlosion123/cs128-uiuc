#include "seam_carver.hpp"
#include "pixel.hpp"
#include "image_ppm.hpp"
#include <iostream>


// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

const ImagePPM& SeamCarver::GetImage() const {
  return image_;
}

int SeamCarver::GetHeight() const {
  return height_;
}

int SeamCarver::GetWidth() const {
  return width_;
}



int SeamCarver::GetEnergy(int row, int col) const {
  Pixel left_pixel = image_.GetPixel(row, (col - 1 + width_) % width_);
  Pixel right_pixel = image_.GetPixel(row, (col + 1) % width_);
  Pixel up_pixel = image_.GetPixel((row - 1 + height_) % height_, col);
  Pixel down_pixel = image_.GetPixel((row + 1) % height_, col);
  int x_diff = (left_pixel.GetRed() - right_pixel.GetRed()) * (left_pixel.GetRed() - right_pixel.GetRed()) +
               (left_pixel.GetGreen() - right_pixel.GetGreen()) * (left_pixel.GetGreen() - right_pixel.GetGreen()) +
               (left_pixel.GetBlue() - right_pixel.GetBlue()) * (left_pixel.GetBlue() - right_pixel.GetBlue());
  int y_diff = (up_pixel.GetRed() - down_pixel.GetRed()) * (up_pixel.GetRed() - down_pixel.GetRed()) +
               (up_pixel.GetGreen() - down_pixel.GetGreen()) * (up_pixel.GetGreen() - down_pixel.GetGreen()) +
               (up_pixel.GetBlue() - down_pixel.GetBlue()) * (up_pixel.GetBlue() - down_pixel.GetBlue());
  return x_diff + y_diff;
}

int** SeamCarver::CreateEnergyMatrix() const {
  int** energy_matrix = new int*[height_];
  for (int row = 0; row < height_; row++) {
    energy_matrix[row] = new int[width_];
    for (int col = 0; col < width_; col++) {
      energy_matrix[row][col] = GetEnergy(row, col);
    }
  }
  return energy_matrix;
}

int** SeamCarver::CreateHorizontalDPMatrix(int**& energy_matrix) const {
  int** horizontal_dp_matrix = new int*[height_];
  for (int row = 0; row < height_; row++) {
    horizontal_dp_matrix[row] = new int[width_];
    for (int col = 0; col < width_; col++) {
      if (col == width_ - 1) {
        horizontal_dp_matrix[row][col] = energy_matrix[row][col];
      } else {
        horizontal_dp_matrix[row][col] = 0;
      }
    }
  }
  return horizontal_dp_matrix;
}

int** SeamCarver::CreateVerticalDPMatrix(int**& energy_matrix) const {
  int** vertical_dp_matrix = new int*[height_];
  for (int row = 0; row < height_; row++) {
    vertical_dp_matrix[row] = new int[width_];
    for (int col = 0; col < width_; col++) {
      if (row == height_ - 1) {
        vertical_dp_matrix[row][col] = energy_matrix[row][col];
      } else {
        vertical_dp_matrix[row][col] = 0;
      }
    }
  }
  return vertical_dp_matrix;
}

int SeamCarver::FindMinHorizontalStartingIndex(int** horizontal_dp_matrix) const {
  int min_index = 0;
  int min_value = horizontal_dp_matrix[0][0];
  for (int row = 1; row < height_; ++row) {
    if (horizontal_dp_matrix[row][0] < min_value) {
      min_value = horizontal_dp_matrix[row][0];
      min_index = row;
    }
  }
  return min_index;
}

int* SeamCarver::GetHorizontalSeam() const {
  int** energy_matrix = CreateEnergyMatrix();
  int** dp_matrix = CreateHorizontalDPMatrix(energy_matrix);
  for (int col = width_ - 2; col >= 0; --col) {
    for (int row = 0; row < height_; ++row) {
      int min = dp_matrix[row][col + 1];
      if (row != 0 && dp_matrix[row - 1][col + 1] < min) {
        min = dp_matrix[row - 1][col + 1];
      }
      if (row != height_ - 1 && dp_matrix[row + 1][col + 1] < min) {
        min = dp_matrix[row + 1][col + 1];
      }
      dp_matrix[row][col] = energy_matrix[row][col] + min;
    }
  }
  int* seam = new int[width_];
  seam[0] = FindMinHorizontalStartingIndex(dp_matrix);
  for (int col = 1; col < width_; ++col) {
    int current_row = seam[col - 1];
    int min = dp_matrix[current_row][col];
    int index = current_row;
    if (current_row != 0 && dp_matrix[current_row - 1][col] < min) {
      min = dp_matrix[current_row - 1][col];
      index = current_row - 1;
    }
    if (current_row != height_ - 1 && dp_matrix[current_row + 1][col] < min) {
      index = current_row + 1;
    }
    seam[col] = index;
  }
  for (int row = 0; row < height_; ++row) {
    delete[] energy_matrix[row];
    delete[] dp_matrix[row];
  }
  delete[] energy_matrix;
  delete[] dp_matrix;
  return seam;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();
  for (int col = 0; col < width_; ++col) {
    for (int row = seam[col]; row < height_ - 1; ++row) {
      image_.SetPixel(row, col, image_.GetPixel(row + 1, col));
    }
  }
  image_.RemoveHorizontalLine();
  height_--;
  image_.SetHeight(height_);
  
  delete[] seam;
}

int SeamCarver::FindVerticalStartingIndex(int** vertical_dp_matrix) const {
  int min_index = 0;
  int min_value = vertical_dp_matrix[0][0];
  for (int col = 1; col < width_; ++col) {
    if (vertical_dp_matrix[0][col] < min_value) {
      min_value = vertical_dp_matrix[0][col];
      min_index = col;
    }
  }
  return min_index;
}

int* SeamCarver::GetVerticalSeam() const {
  int** energy_matrix = CreateEnergyMatrix();
  int** vertical_dp_matrix = CreateVerticalDPMatrix(energy_matrix);
  for (int row = height_ - 2; row >= 0; --row) {
    for (int col = 0; col < width_; ++col) {
      int min = vertical_dp_matrix[row + 1][col];
      if (col != 0 && vertical_dp_matrix[row + 1][col - 1] < min) {
        min = vertical_dp_matrix[row + 1][col - 1];
      }
      if (col != width_ - 1 && vertical_dp_matrix[row + 1][col + 1] < min) {
        min = vertical_dp_matrix[row + 1][col + 1];
      }
      vertical_dp_matrix[row][col] = energy_matrix[row][col] + min;
    }
  }
  int* seam = new int[height_];
  seam[0] = FindVerticalStartingIndex(vertical_dp_matrix);
  for (int row = 1; row < height_; ++row) {
    int current_col = seam[row - 1];
    int min = vertical_dp_matrix[row][current_col];
    int index = current_col;
    if (current_col != 0 && vertical_dp_matrix[row][current_col - 1] < min) {
      min = vertical_dp_matrix[row][current_col - 1];
      index = current_col - 1;
    }
    if (current_col != width_ - 1 && vertical_dp_matrix[row][current_col + 1] < min) {
      index = current_col + 1;
    }
    seam[row] = index;
  }
  for (int i = 0; i < height_; i++) {
    delete[] energy_matrix[i];
    delete[] vertical_dp_matrix[i];
  }
  delete[] energy_matrix;
  delete[] vertical_dp_matrix;
  return seam;
}

void SeamCarver::RemoveVerticalSeam() {
  int* seam = GetVerticalSeam();
  for (int row = 0; row < height_; ++row) {
    for (int col = seam[row]; col < width_ - 1; ++col) {
      image_.SetPixel(row, col, image_.GetPixel(row, col + 1));
    }
  }
  width_--;
  image_.SetWidth(width_);
  delete[] seam;
}


