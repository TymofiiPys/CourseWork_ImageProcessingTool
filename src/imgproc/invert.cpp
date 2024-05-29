#include <functional>
#include <thread>

#include "../config/iptconfigmanager.h"
#include "invert.h"

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

void invert_rgb_color_singlethreaded(std::vector<std::vector<std::vector<unsigned int>>> &rgb_image,
                                     const int &start,
                                     const int &end) {
    for (int i = start; i < end; i++) {
        auto &row = rgb_image.at(i);
        for (auto &cell : row) {
            cell.at(0) = 255 - cell.at(0);
            cell.at(1) = 255 - cell.at(1);
            cell.at(2) = 255 - cell.at(2);
        }
    }
}

void invert_color_rgb(std::vector<std::vector<std::vector<unsigned int>>> &rgb_image) {
    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    int rows_per_thread = rgb_image.size() / nthreads;
    std::thread *threads = new std::thread[nthreads];
    for (int i = 0; i < nthreads; ++i) {
        // auto portionBegin = rgb_image.begin() + i * rows_per_thread;
        // auto portionEnd = (i < nthreads - 1) ? rgb_image.begin() + (i + 1) * rows_per_thread
        //                                      : rgb_image.end();
        // std::vector<std::vector<std::vector<unsigned int>>> imagePortion(portionBegin, portionEnd);
        int start = i * rows_per_thread;
        int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : rgb_image.size();
        threads[i] = std::thread(invert_rgb_color_singlethreaded, std::ref(rgb_image), start, end);
    }
    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }
    // invert_rgb_color_singlethreaded(rgb_image);
}

} // namespace Color
} // namespace ImgProc
