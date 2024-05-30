#include "togray.h"

#include <functional>
#include <thread>

#include "../config/iptconfigmanager.h"

void ImgProc::Color::to_gray_singlethreaded(RGBMatrix &rgb_image, const int &start, const int &end) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < rgb_image.cols(); j++) {
            unsigned int r = std::get<0>(rgb_image(i, j));
            unsigned int g = std::get<1>(rgb_image(i, j));
            unsigned int b = std::get<2>(rgb_image(i, j));
            unsigned int gray = 0.299 * r + 0.587 * g + 0.114 * b;
            rgb_image(i, j) = std::make_tuple(gray, gray, gray);
        }
    }
}

void ImgProc::Color::to_gray(RGBMatrix &rgb_image) {
    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    const int rows_per_thread = rgb_image.rows() / nthreads;
    std::thread *threads = new std::thread[nthreads];
    for (int i = 0; i < nthreads; ++i) {
        int start = i * rows_per_thread;
        int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : rgb_image.rows();
        threads[i] = std::thread(to_gray_singlethreaded, std::ref(rgb_image), start, end);
    }
    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }
}
