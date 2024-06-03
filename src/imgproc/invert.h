#ifndef INVERT_H
#define INVERT_H

#include <vector>

#include "aliases.h"

namespace ImgProc {

inline namespace Color {

// TODO: switch grayscale methods to use matrices

void invert_color_singlethreaded(std::vector<std::vector<unsigned int>> &);

void invert_color(std::vector<std::vector<unsigned int>> &image);

void invert_rgb_color_singlethreaded(RGBMatrix &rgb_image,
                                     const bool &red,
                                     const bool &green,
                                     const bool &blue,
                                     const int &start,
                                     const int &end);

void invert_color_rgb(RGBMatrix &rgb_image, const bool &red, const bool &green, const bool &blue);

} // namespace Color

} // namespace ImgProc

#endif // INVERT_H
