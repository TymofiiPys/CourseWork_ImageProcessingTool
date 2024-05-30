#ifndef ALIASES_H
#define ALIASES_H

#include <Eigen/Dense>

namespace ImgProc {

using RGBTuple = std::tuple<unsigned int, unsigned int, unsigned int>;
using RGBMatrix = Eigen::MatrixX<RGBTuple>;

} // namespace ImgProc

#endif // ALIASES_H
