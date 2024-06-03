#ifndef SCALE_H
#define SCALE_H

#include "aliases.h"

namespace ImgProc {

inline namespace Transform {

void scale_img_singlethreaded(const RGBMatrix &image,
                              RGBMatrix &scaled_image,
                              const Eigen::Matrix3d &transform_inv,
                              const int &start,
                              const int &end);

RGBMatrix scale_img(RGBMatrix &image, const double &sX, const double &sY);

} // namespace Transform

} // namespace ImgProc

#endif // SCALE_H
