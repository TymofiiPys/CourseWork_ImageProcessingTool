#ifndef AVERAGING_H
#define AVERAGING_H

#include "aliases.h"

namespace ImgProc {

inline namespace Filter {

const Eigen::Matrix3d matrix{{1, 2, 1}, {2, 3, 2}, {1, 2, 1}};

void weighted_averaging(RGBMatrix &img);

} // namespace Filter

} // namespace ImgProc

#endif // AVERAGING_H
