#ifndef GAUSS_FILTER_H
#define GAUSS_FILTER_H

#include "aliases.h"

namespace ImgProc {

inline namespace Filter {

const int K = 1;

const int sigma_multiply = 3;

void gauss_filter_singlethreaded(const RGBMatrix &img,
                                 RGBMatrix &filtered,
                                 const Eigen::MatrixXd &kernel,
                                 const int box_size,
                                 const int start,
                                 const int end);

Eigen::MatrixXd compute_kernel(const int &box_size);

void gauss_filter(RGBMatrix &img, int box_size);

} // namespace Filter

} // namespace ImgProc

#endif // GAUSS_FILTER_H
