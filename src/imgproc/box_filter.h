#ifndef BOX_FILTER_H
#define BOX_FILTER_H

#include "aliases.h"

namespace ImgProc {

inline namespace Filter {

Eigen::MatrixXd compute_kernel(const int &box_size, const int coef);

void box_filter(RGBMatrix &img, int box_size, const int coef);

} // namespace Filter

} // namespace ImgProc

#endif // BOX_FILTER_H
