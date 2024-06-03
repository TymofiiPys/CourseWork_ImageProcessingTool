#ifndef LAPLACIAN_H
#define LAPLACIAN_H

#include "aliases.h"

namespace ImgProc {

inline namespace Filter {

Eigen::Matrix3d compute_lapl_kernel(const int coef);

void laplacian_singlethreaded(const RGBMatrix &img,
                              RGBMatrix &filtered,
                              const Eigen::MatrixXd &kernel,
                              const int coef,
                              const int start,
                              const int end);

void laplacian(RGBMatrix &img, const int coef);

} // namespace Filter

} // namespace ImgProc

#endif // LAPLACIAN_H
