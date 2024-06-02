#include "gauss_filter.h"

#include <cmath>

Eigen::MatrixXd ImgProc::Filter::compute_kernel(const int &box_size) {
    const double sigma = static_cast<double>(box_size) / ImgProc::Filter::sigma_multiply;
    Eigen::MatrixXd kernel(box_size, box_size);
    const int halfBS = box_size / 2;
    for (int i = 0; i < box_size; ++i) {
        for (int j = 0; j < box_size; ++j) {
            kernel(i, j) = K
                           * std::exp(-((i - halfBS) * (i - halfBS) + (j - halfBS) * (j - halfBS))
                                      / (2 * sigma * sigma));
        }
    }
    return kernel;
}

void ImgProc::Filter::gauss_filter(RGBMatrix &img, int box_size) {
    if (box_size % 2 == 0)
        box_size--;
    Eigen::MatrixXd kernel = compute_kernel(box_size);
    RGBMatrix newimg(img.rows(), img.cols());
    const int halfBS = box_size / 2;
    for (int x = 0; x < newimg.rows(); ++x) {
        for (int y = 0; y < newimg.cols(); ++y) {
            double weightR = 0;
            double weightG = 0;
            double weightB = 0;
            double denum = 0;
            for (int i = -halfBS; i <= halfBS; ++i) {
                if (x + i >= img.rows() || x + i < 0)
                    continue;
                for (int j = -halfBS; j <= halfBS; ++j) {
                    if (y + j >= img.cols() || y + j < 0)
                        continue;
                    weightR += kernel(i + halfBS, j + halfBS) * std::get<0>(img(x - i, y - j));
                    weightG += kernel(i + halfBS, j + halfBS) * std::get<1>(img(x - i, y - j));
                    weightB += kernel(i + halfBS, j + halfBS) * std::get<2>(img(x - i, y - j));
                    denum += kernel(i + halfBS, j + halfBS);
                }
            }
            std::get<0>(newimg(x, y)) = weightR / denum;
            std::get<1>(newimg(x, y)) = weightG / denum;
            std::get<2>(newimg(x, y)) = weightB / denum;
        }
    }
    img = newimg;
}