#ifndef BOX_FILTER_H
#define BOX_FILTER_H

#include "aliases.h"

namespace ImgProc {

inline namespace Filter {

void box_filter_singlethreaded(const RGBMatrix &img,
                               RGBMatrix &filtered,
                               const Eigen::MatrixXd &kernel,
                               const int box_size,
                               const int start,
                               const int end);

Eigen::MatrixXd compute_kernel(const int &box_size, const double coef);

void box_filter(RGBMatrix &img, int box_size, const double coef);

} // namespace Filter

} // namespace ImgProc

#endif // BOX_FILTER_H
