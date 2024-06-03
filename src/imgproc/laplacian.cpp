#include "laplacian.h"

#include "../config/iptconfigmanager.h"

const int kLaplBoxSize = 3;

Eigen::Matrix3d ImgProc::Filter::compute_lapl_kernel(const int coef) {
    Eigen::Matrix3d ret;
    ret << 0, 1, 0, 1, -4, 1, 0, 1, 0;
    return ret;
}

void ImgProc::laplacian_singlethreaded(const RGBMatrix &img,
                                       RGBMatrix &filtered,
                                       const Eigen::MatrixXd &kernel,
                                       const int coef,
                                       const int start,
                                       const int end) {
    const int halfBS = kLaplBoxSize / 2;
    for (int x = start; x < end; ++x) {
        for (int y = 0; y < filtered.cols(); ++y) {
            bool broken = false;
            double derivR = 0;
            double derivG = 0;
            double derivB = 0;
            for (int i = -halfBS; i <= halfBS; ++i) {
                // do not filter pixels which lie in first or last column or row
                if (broken || x - i >= img.rows() || x - i < 0) {
                    broken = true;
                    break;
                }
                for (int j = -halfBS; j <= halfBS; ++j) {
                    if (y - j >= img.cols() || y - j < 0) {
                        broken = true;
                        break;
                    }
                    derivR += kernel(i + halfBS, j + halfBS) * std::get<0>(img(x - i, y - j));
                    derivG += kernel(i + halfBS, j + halfBS) * std::get<1>(img(x - i, y - j));
                    derivB += kernel(i + halfBS, j + halfBS) * std::get<2>(img(x - i, y - j));
                }
            }
            if (broken) {
                std::get<0>(filtered(x, y)) = std::get<0>(img(x, y));
                std::get<1>(filtered(x, y)) = std::get<1>(img(x, y));
                std::get<2>(filtered(x, y)) = std::get<2>(img(x, y));
            } else {
                std::get<0>(filtered(x, y)) = std::clamp(std::get<0>(img(x, y))
                                                             - static_cast<unsigned int>(derivR
                                                                                         * coef),
                                                         0u,
                                                         255u);
                std::get<1>(filtered(x, y)) = std::clamp(std::get<1>(img(x, y))
                                                             - static_cast<unsigned int>(derivG
                                                                                         * coef),
                                                         0u,
                                                         255u);
                std::get<2>(filtered(x, y)) = std::clamp(std::get<2>(img(x, y))
                                                             - static_cast<unsigned int>(derivB
                                                                                         * coef),
                                                         0u,
                                                         255u);
            }
        }
    }
}

void ImgProc::Filter::laplacian(RGBMatrix &img, const int coef) {
    Eigen::MatrixXd kernel = compute_lapl_kernel(coef);
    RGBMatrix filtered(img.rows(), img.cols());
    const int halfBS = kLaplBoxSize / 2;

    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    int rows_per_thread = filtered.rows() / nthreads;
    std::thread *threads = new std::thread[nthreads];
    // laplacian_singlethreaded(img, filtered, kernel, coef, 0, filtered.rows());
    for (int i = 0; i < nthreads; ++i) {
        int start = i * rows_per_thread;
        int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : filtered.rows();
        threads[i] = std::thread(laplacian_singlethreaded,
                                 std::ref(img),
                                 std::ref(filtered),
                                 std::ref(kernel),
                                 coef,
                                 start,
                                 end);
    }

    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }

    img = filtered;
}
