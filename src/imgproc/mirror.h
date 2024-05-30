#ifndef MIRROR_H
#define MIRROR_H

#include <vector>

namespace ImgProc {

inline namespace Color {

void mirror_singlethreaded(std::vector<std::vector<std::vector<unsigned int>>> &rgb_image,
                           const int &start,
                           const int &end,
                           bool dir);

void mirror(std::vector<std::vector<std::vector<unsigned int>>> &rgb_image);

} // namespace Color

} // namespace ImgProc

#endif // MIRROR_H
