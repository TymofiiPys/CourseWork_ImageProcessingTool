#include <thread>

#include "../config/iptconfigmanager.h"
#include "invert.h"

namespace ImgProc {

inline namespace Color {
void invert_color_singlethreaded(std::vector<std::vector<int>> &image) {
    for (auto &row : image) {
        for (auto &cell : row) {
            cell = 255 - cell;
        }
    }
}

void invert_color(std::vector<std::vector<int>> &image) {
    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    int rows_per_thread = image.size() / nthreads;
    std::thread *threads = new std::thread[nthreads];
    for (int i = 0; i < nthreads; ++i) {
        threads[i] = std::thread(invert_color_singlethreaded,
                                 std::vector<std::vector<int>>(image.begin() + i * rows_per_thread,
                                                               image.begin()
                                                                   + (i + 1) * rows_per_thread));
    }
    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }
}

void invert_color_rgb(std::vector<std::vector<std::vector<int>>> &rgb_image) {
    for (int i = 0; i < 3; i++) {
        invert_color(rgb_image.at(i));
    }
}

} // namespace Color
} // namespace ImgProc
