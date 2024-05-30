#include "logtransform.h"

#include <cmath>

#include <algorithm>
#include <functional>
#include <thread>

#include "../config/iptconfigmanager.h"

void ImgProc::Color::log_transform_singlethreaded(RGBMatrix &image,
                                                  const int &start,
                                                  const int &end,
                                                  const double &c,
                                                  const bool &red,
                                                  const bool &green,
                                                  const bool &blue) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < image.cols(); j++) {
            if (red) {
                unsigned int &r = std::get<0>(image(i, j));
                r = std::clamp(static_cast<unsigned int>(c * std::log(1 + r)), 0u, 255u);
                // qDebug() << r;
            }
            if (green) {
                unsigned int &g = std::get<1>(image(i, j));
                g = std::clamp(static_cast<unsigned int>(c * std::log(1 + g)), 0u, 255u);
                // qDebug() << g;
            }
            if (blue) {
                unsigned int &b = std::get<2>(image(i, j));
                b = std::clamp(static_cast<unsigned int>(c * std::log(1 + b)), 0u, 255u);
                // qDebug() << b;
            }
        }
    }
}

void ImgProc::Color::log_transform(
    RGBMatrix &image, const double &c, const bool &red, const bool &green, const bool &blue) {
    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    const int rows_per_thread = image.rows() / nthreads;
    std::thread *threads = new std::thread[nthreads];
    for (int i = 0; i < nthreads; ++i) {
        int start = i * rows_per_thread;
        int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : image.rows();
        threads[i] = std::thread(log_transform_singlethreaded,
                                 std::ref(image),
                                 start,
                                 end,
                                 std::ref(c),
                                 std::ref(red),
                                 std::ref(green),
                                 std::ref(blue));
    }
    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }
}
