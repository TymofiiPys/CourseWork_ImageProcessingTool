#include "histeq.h"

#include <array>

#include <QDebug>

std::array<std::array<unsigned int, 256>, 3> histogram(const ImgProc::RGBMatrix &image) {
    // histogram for each of 3 color channels;
    std::array<std::array<unsigned int, 256>, 3> ret;
    for (std::array<unsigned int, 256> &chan : ret) {
        for (unsigned int &color : chan) {
            color = 0;
        }
    }
    for (int x = 0; x < image.rows(); ++x) {
        for (int y = 0; y < image.cols(); ++y) {
            const unsigned int &r = std::get<0>(image(x, y));
            const unsigned int &g = std::get<1>(image(x, y));
            const unsigned int &b = std::get<2>(image(x, y));
            ret.at(0).at(r)++;
            ret.at(1).at(g)++;
            ret.at(2).at(b)++;
        }
    }

    return ret;
}

void ImgProc::hist_eq(RGBMatrix &image) {
    std::array<std::array<unsigned int, 256>, 3> hist = histogram(image);
    std::unordered_map<unsigned int, unsigned int> colorNewVal_R;
    std::unordered_map<unsigned int, unsigned int> colorNewVal_G;
    std::unordered_map<unsigned int, unsigned int> colorNewVal_B;

    for (int x = 0; x < image.rows(); ++x) {
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

    for (int x = 0; x < image.rows(); ++x) {
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

    for (int x = 0; x < image.rows(); ++x) {
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
