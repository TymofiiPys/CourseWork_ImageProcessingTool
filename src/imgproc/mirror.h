#ifndef MIRROR_H
#define MIRROR_H

#include "aliases.h"

namespace ImgProc {

inline namespace Transform {

void mirror_hor_singlethreaded(RGBMatrix &rgb_image, const int &start, const int &end);
void mirror_ver_singlethreaded(RGBMatrix &rgb_image, const int &start, const int &end);

void mirror(RGBMatrix &rgb_image, const bool &dir);

} // namespace Transform

} // namespace ImgProc

#endif // MIRROR_H
