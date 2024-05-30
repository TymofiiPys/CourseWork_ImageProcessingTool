#include "mirror.h"

#include "../config/iptconfigmanager.h"

void ImgProc::Transform::mirror_hor_singlethreaded(RGBMatrix &rgb_image,
                                                   const int &start,
                                                   const int &end) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < rgb_image.cols() / 2; j++) {
            std::swap(rgb_image(i, j), rgb_image(i, rgb_image.cols() - 1 - j));
        }
    }
}

void ImgProc::Transform::mirror_ver_singlethreaded(RGBMatrix &rgb_image,
                                                   const int &start,
                                                   const int &end) {
    for (int j = start; j < end; j++) {
        for (int i = 0; i < rgb_image.rows() / 2; i++) {
            std::swap(rgb_image(i, j), rgb_image(rgb_image.rows() - 1 - i, j));
        }
    }
}

void ImgProc::Transform::mirror(RGBMatrix &rgb_image, const bool &horizontal) {
    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    const int rows_per_thread = rgb_image.rows() / nthreads;
    const int cols_per_thread = rgb_image.cols() / nthreads;
    const int &step = horizontal ? rows_per_thread : cols_per_thread;
    const int &fin = horizontal ? rgb_image.rows() : rgb_image.cols();
    std::thread *threads = new std::thread[nthreads];
    for (int i = 0; i < nthreads; ++i) {
        int start = i * step;
        int end = (i < nthreads - 1) ? (i + 1) * step : fin;
        if (horizontal) {
            threads[i] = std::thread(mirror_hor_singlethreaded, std::ref(rgb_image), start, end);
        } else {
            threads[i] = std::thread(mirror_ver_singlethreaded, std::ref(rgb_image), start, end);
        }
    }

    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }
}
