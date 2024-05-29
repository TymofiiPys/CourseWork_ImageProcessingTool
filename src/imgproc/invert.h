#ifndef INVERT_H
#define INVERT_H

#include <vector>

namespace ImgProc {

inline namespace Color {

void invert_color_singlethreaded(std::vector<std::vector<int>> &);

void invert_color(std::vector<std::vector<int>> &image);

void invert_color_rgb(std::vector<std::vector<std::vector<int>>> &rgb_image);

} // namespace Color

} // namespace ImgProc

#endif // INVERT_H
