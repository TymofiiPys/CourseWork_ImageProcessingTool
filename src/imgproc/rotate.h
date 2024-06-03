#ifndef ROTATE_H
#define ROTATE_H

#include "aliases.h"

namespace ImgProc {

inline namespace Transform {

void rotate_img_singlethreaded(const RGBMatrix &image,
                               RGBMatrix &rotated_image,
                               const Eigen::Matrix3d &rotation,
                               const Eigen::Matrix3d &translation_before,
                               const Eigen::Matrix3d &translation_after,
                               const int &start,
                               const int &end);

// void rotate_img_singlethreaded_gray(const std::vector<std::vector<unsigned int>> &image,
//                                     std::vector<std::vector<unsigned int>> &rotated_image,
//                                     const Eigen::Matrix3d &transform_inverse,
//                                     const int &start,
//                                     const int &end);

void rotate_img(RGBMatrix &image, double &angle);

// void rotate_img_gray(std::vector<std::vector<unsigned int>> &image, double &angle);

} // namespace Transform

} // namespace ImgProc

#endif // ROTATE_H
