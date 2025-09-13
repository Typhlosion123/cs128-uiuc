#ifndef PATH_IMAGE_H
#define PATH_IMAGE_H

#include <cstdlib>  // for size_t
#include <vector>

#include "color.hpp"
#include "path.hpp"
#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"


class PathImage {
public:
  PathImage(const GrayscaleImage& image, const ElevationDataset& dataset);

  size_t Width() const {return width_; }
  size_t Height() const {return height_; }
  unsigned int MaxColorvalue() const {return kMaxColorValue; }
  const std::vector<Path>& Paths() const {return paths_; }
  const std::vector<std::vector<Color>>& GetPathImage() const {return path_image_; }

  void ToPpm(const std::string& name);

private:
  std::vector<Path> paths_;
  size_t width_ = 0;
  size_t height_ = 0;
  std::vector<std::vector<Color>> path_image_;
  static const int kMaxColorValue = 255;

  
};

#endif