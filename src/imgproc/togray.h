#ifndef TOGRAY_H
#define TOGRAY_H

#include "aliases.h"

namespace ImgProc {

inline namespace Color {

void to_gray_singlethreaded(RGBMatrix &rgb_image, const int &start, const int &end);

void to_gray(RGBMatrix &rgb_image);

} // namespace Color

} // namespace ImgProc

#endif // TOGRAY_H
