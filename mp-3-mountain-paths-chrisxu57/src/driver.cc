#include <iostream>

#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"
#include "path_image.hpp"

int main() {
  const std::string kTestInputFile =
      "./example-data/ex_input_data/map-input-w844-h480.dat";
  constexpr size_t kTestInputFileWidth = 844;
  constexpr size_t kTestInputFileHeight = 480;

  ElevationDataset ed(
      kTestInputFile, kTestInputFileWidth, kTestInputFileHeight);
  
  std::cout << ed.MaxEle() << " " << ed.MinEle() << std::endl;
  GrayscaleImage gi2(ed);
  gi2.ToPpm("hello.ppm");
  PathImage pi(gi2, ed);
  pi.ToPpm("goodbye.ppm");
}