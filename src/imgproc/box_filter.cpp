#include "box_filter.h"

#include "../config/iptconfigmanager.h"

Eigen::MatrixXd ImgProc::Filter::compute_kernel(const int &box_size, const int coef) {
    Eigen::MatrixXd kernel(box_size, box_size);
    for (int i = 0; i < box_size; ++i) {
        for (int j = 0; j < box_size; ++j) {
            kernel(i, j) = coef;
        }
    }
    return kernel;
}

void ImgProc::box_filter_singlethreaded(const RGBMatrix &img,
                                        RGBMatrix &filtered,
                                        const Eigen::MatrixXd &kernel,
                                        const int box_size,
                                        const int start,
                                        const int end) {
    const int halfBS = box_size / 2;
    for (int x = start; x < end; ++x) {
        for (int y = 0; y < filtered.cols(); ++y) {
            double weightR = 0;
            double weightG = 0;
            double weightB = 0;
            double denum = 0;
            for (int i = -halfBS; i <= halfBS; ++i) {
                if (x - i >= img.rows() || x - i < 0)
                    continue;
                for (int j = -halfBS; j <= halfBS; ++j) {
                    if (y - j >= img.cols() || y - j < 0)
                        continue;
                    weightR += kernel(i + halfBS, j + halfBS) * std::get<0>(img(x - i, y - j));
                    weightG += kernel(i + halfBS, j + halfBS) * std::get<1>(img(x - i, y - j));
                    weightB += kernel(i + halfBS, j + halfBS) * std::get<2>(img(x - i, y - j));
                    denum += kernel(i + halfBS, j + halfBS);
                }
            }
            std::get<0>(filtered(x, y)) = weightR / denum;
            std::get<1>(filtered(x, y)) = weightG / denum;
            std::get<2>(filtered(x, y)) = weightB / denum;
        }
    }
}

void ImgProc::Filter::box_filter(RGBMatrix &img, int box_size, const int coef) {
    if (box_size % 2 == 0)
        box_size--;
    Eigen::MatrixXd kernel = compute_kernel(box_size, coef);
    RGBMatrix filtered(img.rows(), img.cols());

    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    int rows_per_thread = filtered.rows() / nthreads;
    std::thread *threads = new std::thread[nthreads];

    for (int i = 0; i < nthreads; ++i) {
        int start = i * rows_per_thread;
        int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : filtered.rows();
        threads[i] = std::thread(box_filter_singlethreaded,
                                 std::ref(img),
                                 std::ref(filtered),
                                 std::ref(kernel),
                                 box_size,
                                 start,
                                 end);
    }

    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }

    img = filtered;
}
