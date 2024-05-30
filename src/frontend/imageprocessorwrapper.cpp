#include "imageprocessorwrapper.h"

#include <QColor>
#include <QDebug>

#include "../imgproc/invert.h"
#include "../imgproc/rotate.h"

std::vector<std::vector<std::vector<uint>>> ImageProcessorWrapper::rgbImageToVector(
    const QImage &from) {
    if (!from.isGrayscale()) {
        std::vector<std::vector<std::vector<uint>>> ret;
        for (int y = 0; y < from.height(); y++) {
            ret.push_back(std::vector<std::vector<uint>>());
            const QRgb *line = reinterpret_cast<const QRgb *>(from.scanLine(y));

            for (int x = 0; x < from.width(); ++x) {
                const QColor pixel = QColor(line[x]);
                std::vector<uint> pixelV = {static_cast<uint>(pixel.red()),
                                            static_cast<uint>(pixel.blue()),
                                            static_cast<uint>(pixel.green())};
                ret.at(y).push_back(std::move(pixelV));
            }
        }
        return ret;
    }
    return std::vector<std::vector<std::vector<uint>>>();
}

std::vector<std::vector<uint>> ImageProcessorWrapper::imageToVector(const QImage &from) {
    if (from.isGrayscale()) {
        std::vector<std::vector<uint>> ret;
        for (int y = 0; y < from.height(); y++) {
            ret.push_back(std::vector<uint>());
            const uchar *line = from.scanLine(y);

            for (int x = 0; x < from.width(); ++x) {
                ret.at(y).push_back(static_cast<uint>(line[x]));
            }
        }
        return ret;
    }
    return std::vector<std::vector<uint>>();
}

void ImageProcessorWrapper::vectorToImage(QImage &dest, const std::vector<std::vector<uint>> &src) {
    // TODO: grayscale conversion
    if (dest.isGrayscale()) {
        for (int y = 0; y < src.size(); ++y) {
            const std::vector<uint> &row = src.at(y);
            uchar *line = dest.scanLine(y);
            for (int x = 0; x < row.size(); ++x) {
                const uint &cell = row.at(x);
                line[x] = static_cast<uchar>(cell);
            }
        }
    }
}

void ImageProcessorWrapper::vectorToRgbImage(
    QImage &dest, const std::vector<std::vector<std::vector<uint>>> &src) {
    if (!dest.isGrayscale()) {
        for (int y = 0; y < src.size(); ++y) {
            const std::vector<std::vector<uint>> &row = src.at(y);
            QRgb *line = reinterpret_cast<QRgb *>(dest.scanLine(y));
            for (int x = 0; x < row.size(); ++x) {
                const std::vector<uint> &cell = row.at(x);
                line[x] = qRgb(cell.at(0), cell.at(1), cell.at(2));
            }
        }
    }
}

void ImageProcessorWrapper::invertColor(QImage &img) {
    std::vector<std::vector<std::vector<uint>>> imageV = rgbImageToVector(img);
    ImgProc::Color::invert_color_rgb(imageV);
    // std::vector<std::vector<std::vector<uint>>> imageV = {{{0u, 0u, 0u}, {255u, 255u, 255u}},
    //                                                       {{255u, 255u, 255u}, {128u, 128u, 128u}}};
    vectorToRgbImage(img, imageV);
}

QImage ImageProcessorWrapper::rotateImage(QImage &img, double &angle) {
    std::vector<std::vector<std::vector<uint>>> imageV = rgbImageToVector(img);
    ImgProc::Transform::rotate_img(imageV, angle);
    QImage rotated(imageV[0].size(), imageV.size(), img.format());
    vectorToRgbImage(rotated, imageV);
    return rotated;
    // std::vector<std::vector<uint>> imageV = imageToVector(img);
    // ImgProc::Transform::rotate_img_gray(imageV, angle);
    // QImage rotated(imageV[0].size(), imageV.size(), QImage::Format_ARGB32);
    // vectorToImage(rotated, imageV);
    // return rotated;
}
