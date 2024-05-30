#include "histeq.h"

#include <array>
#include <functional>
#include <mutex>
#include <thread>

#include <QDebug>

#include "../config/iptconfigmanager.h"

std::mutex hist_mutex;

void compute_histogram(const ImgProc::RGBMatrix &image,
                       ImgProc::RGBHistogram &hist,
                       const int &start,
                       const int &end) {
    for (int x = start; x < end; ++x) {
        for (int y = 0; y < image.cols(); ++y) {
            const unsigned int &r = std::get<0>(image(x, y));
            const unsigned int &g = std::get<1>(image(x, y));
            const unsigned int &b = std::get<2>(image(x, y));
            // lock because same channel value may be met in different threads :)
            hist_mutex.lock();
            hist.at(0).at(r)++;
            hist.at(1).at(g)++;
            hist.at(2).at(b)++;
            hist_mutex.unlock();
        }
    }
}

void ImgProc::hist_eq_singlethreaded(RGBMatrix &image,
                                     const RGBHistogram &hist,
                                     const int &start,
                                     const int &end,
                                     const bool &red,
                                     const bool &green,
                                     const bool &blue) {
    if (red) {
        std::unordered_map<unsigned int, unsigned int> colorNewVal_R;

        for (int x = start; x < end; ++x) {
            for (int y = 0; y < image.cols(); ++y) {
                int pixelRValue = std::get<0>(image(x, y));
                if (colorNewVal_R.count(pixelRValue) == 0) {
                    int integral = 0;
                    for (int shade = 0; shade <= pixelRValue; shade++) {
                        integral += hist.at(0).at(shade);
                    }
                    colorNewVal_R.insert(
                        {pixelRValue, (integral * 255 / (image.rows() * image.cols()))});
                }
                std::get<0>(image(x, y)) = colorNewVal_R.at(pixelRValue);
            }
        }
    }
    if (green) {
        std::unordered_map<unsigned int, unsigned int> colorNewVal_G;

        for (int x = start; x < end; ++x) {
            for (int y = 0; y < image.cols(); ++y) {
                int pixelGValue = std::get<1>(image(x, y));
                if (colorNewVal_G.count(pixelGValue) == 0) {
                    int integral = 0;
                    for (int shade = 0; shade <= pixelGValue; shade++) {
                        integral += hist.at(1).at(shade);
                    }
                    colorNewVal_G.insert(
                        {pixelGValue, (integral * 255 / (image.rows() * image.cols()))});
                }
                std::get<1>(image(x, y)) = colorNewVal_G.at(pixelGValue);
            }
        }
    }
    if (blue) {
        std::unordered_map<unsigned int, unsigned int> colorNewVal_B;

        for (int x = start; x < end; ++x) {
            for (int y = 0; y < image.cols(); ++y) {
                int pixelBValue = std::get<2>(image(x, y));
                if (colorNewVal_B.count(pixelBValue) == 0) {
                    int integral = 0;
                    for (int shade = 0; shade <= pixelBValue; shade++) {
                        integral += hist.at(2).at(shade);
                    }
                    colorNewVal_B.insert(
                        {pixelBValue, (integral * 255 / (image.rows() * image.cols()))});
                }
                std::get<2>(image(x, y)) = colorNewVal_B.at(pixelBValue);
            }
        }
    }
}

void ImgProc::hist_eq(RGBMatrix &image, const bool &red, const bool &green, const bool &blue) {
    // histogram for each of 3 color channels;
    std::array<std::array<unsigned int, 256>, 3> hist;
    // histogram for each of 3 color channels;
    for (std::array<unsigned int, 256> &chan : hist) {
        for (unsigned int &color : chan) {
            color = 0;
        }
    }

    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    const int rows_per_thread = image.rows() / nthreads;

    std::thread *threads = new std::thread[nthreads];
    for (int i = 0; i < nthreads; ++i) {
        int start = i * rows_per_thread;
        int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : image.rows();
        threads[i] = std::thread(compute_histogram, std::ref(image), std::ref(hist), start, end);
    }
    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }

    threads = new std::thread[nthreads];
    for (int i = 0; i < nthreads; ++i) {
        int start = i * rows_per_thread;
        int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : image.rows();
        threads[i] = std::thread(hist_eq_singlethreaded,
                                 std::ref(image),
                                 std::ref(hist),
                                 start,
                                 end,
                                 std::ref(red),
                                 std::ref(green),
                                 std::ref(blue));
    }
    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }
}
