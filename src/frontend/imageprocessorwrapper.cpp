#include "imageprocessorwrapper.h"

#include <QColor>
#include <QDebug>

#include "../imgproc/invert.h"

std::vector<std::vector<std::vector<uchar>>> ImageProcessorWrapper::rgbImageToVector(
    const QImage &from) {
    if (!from.isGrayscale()) {
        std::vector<std::vector<std::vector<uchar>>> ret;
        for (int y = 0; y < from.height(); y++) {
            ret.push_back(std::vector<std::vector<uchar>>());
            const QRgb *line = reinterpret_cast<const QRgb *>(from.scanLine(y));

            for (int x = 0; x < from.width(); ++x) {
                const QColor pixel = QColor(line[x]);
                std::vector<uchar> pixelV = {static_cast<uchar>(pixel.red()),
                                             static_cast<uchar>(pixel.blue()),
                                             static_cast<uchar>(pixel.green())};
                ret.at(y).push_back(std::move(pixelV));
            }
        }
        return ret;
    }
    return std::vector<std::vector<std::vector<uchar>>>();
}

std::vector<std::vector<uint>> ImageProcessorWrapper::imageToVector(const QImage &from) {
    if (from.isGrayscale()) {
        std::vector<std::vector<uint>> ret;
        for (int i = 0; i < from.height(); i++) {
            ret.push_back(std::vector<uint>());
            for (int j = 0; j < from.width(); ++j) {
                const QRgb &pixelQ = from.pixel(j, i);
                uint pixelV = static_cast<uint>(qGray(pixelQ));
                ret.at(i).push_back(std::move(pixelV));
            }
        }
        return ret;
    }
    return std::vector<std::vector<uint>>();
}

void ImageProcessorWrapper::vectorToImage(QImage &dest, const std::vector<std::vector<uint>> &src) {
    // TODO: grayscale conversion
}

void ImageProcessorWrapper::vectorToRgbImage(
    QImage &dest, const std::vector<std::vector<std::vector<uchar>>> &src) {
    if (!dest.isGrayscale()) {
        for (int y = 0; y < src.size(); ++y) {
            const std::vector<std::vector<uchar>> &row = src.at(y);
            QRgb *line = reinterpret_cast<QRgb *>(dest.scanLine(y));
            for (int x = 0; x < row.size(); ++x) {
                const std::vector<uchar> &cell = row.at(x);
                dest.setPixelColor(x, y, QColor::fromRgb(cell.at(0), cell.at(1), cell.at(2)));
                // line[x] = qRgb(cell.at(0), cell.at(1), cell.at(2));
            }
        }
    }
}

void ImageProcessorWrapper::invertColor(QImage &img) {
    std::vector<std::vector<std::vector<uchar>>> imageV = rgbImageToVector(img);
    ImgProc::Color::invert_color_rgb(imageV);
    // std::vector<std::vector<std::vector<uint>>> imageV = {{{0u, 0u, 0u}, {255u, 255u, 255u}},
    //                                                       {{255u, 255u, 255u}, {128u, 128u, 128u}}};
    vectorToRgbImage(img, imageV);
}
