#include "invert.h"

#include <functional>
#include <thread>

#include "../config/iptconfigmanager.h"

namespace ImgProc {

inline namespace Color {
void invert_color_singlethreaded(std::vector<std::vector<unsigned int>> &image) {
    for (auto &row : image) {
        for (auto &cell : row) {
            cell = 255 - cell;
        }
    }
}

void invert_color(std::vector<std::vector<unsigned int>> &image) {
    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    int rows_per_thread = image.size() / nthreads;
    std::thread *threads = new std::thread[nthreads];
    for (int i = 0; i < nthreads; ++i) {
        auto portionBegin = image.begin() + i * rows_per_thread;
        auto portionEnd = image.begin() + (i + 1) * rows_per_thread;
        std::vector<std::vector<unsigned int>> imagePortion(portionBegin, portionEnd);
        threads[i] = std::thread(invert_color_singlethreaded, std::ref(imagePortion));
    }
    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }
}

void invert_rgb_color_singlethreaded(RGBMatrix &rgb_image,
                                     const bool &red,
                                     const bool &green,
                                     const bool &blue,
                                     const int &start,
                                     const int &end) {
    for (int i = start; i < end; i++) {
        for (int j = 0; j < rgb_image.cols(); j++) {
            if (red) {
                unsigned int &r = std::get<0>(rgb_image(i, j));
                r = 255 - r;
            }
            if (green) {
                unsigned int &g = std::get<1>(rgb_image(i, j));
                g = 255 - g;
            }
            if (blue) {
                unsigned int &b = std::get<2>(rgb_image(i, j));
                b = 255 - b;
            }
        }
    }
}

void invert_color_rgb(RGBMatrix &rgb_image, const bool &red, const bool &green, const bool &blue) {
    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    const int rows_per_thread = rgb_image.rows() / nthreads;
    std::thread *threads = new std::thread[nthreads];
    for (int i = 0; i < nthreads; ++i) {
        int start = i * rows_per_thread;
        int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : rgb_image.rows();
        threads[i] = std::thread(invert_rgb_color_singlethreaded,
                                 std::ref(rgb_image),
                                 std::ref(red),
                                 std::ref(green),
                                 std::ref(blue),
                                 start,
                                 end);
    }
    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }
}

} // namespace Color
} // namespace ImgProc
