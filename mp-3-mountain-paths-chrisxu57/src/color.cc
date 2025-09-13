#include "color.hpp"

#include <iostream>
#include <stdexcept>

Color::Color(int r, int g, int b): red_(r), green_(g), blue_(b) {
  if (r < kColorValueMin || r > kColorValueMax || g < kColorValueMin || g > kColorValueMax 
      || b < kColorValueMin|| b > kColorValueMax) {
    throw std::runtime_error("color is out of range");
  }
  red_ = r;
  green_ = g;
  blue_ = b;
}

// do not modify
bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}