#include "imageprocessorwrapper.h"

#include <QColor>
#include <QDebug>

#include "../imgproc/invert.h"
#include "../imgproc/mirror.h"
#include "../imgproc/rotate.h"
#include "../imgproc/togray.h"

Eigen::MatrixX<RGBTuple> ImageProcessorWrapper::rgbImageToMatrix(const QImage &from) {
    if (!from.isGrayscale()) {
        Eigen::MatrixX<RGBTuple> ret(from.height(), from.width());
        for (int y = 0; y < from.height(); y++) {
            const QRgb *line = reinterpret_cast<const QRgb *>(from.scanLine(y));

            for (int x = 0; x < from.width(); ++x) {
                const QColor pixel = QColor(line[x]);
                ret(y, x) = std::make_tuple(static_cast<uint>(pixel.red()),
                                            static_cast<uint>(pixel.green()),
                                            static_cast<uint>(pixel.blue()));
            }
        }
        return ret;
    }
    return Eigen::Matrix<RGBTuple, 1, 1>();
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

void ImageProcessorWrapper::matrixToRgbImage(QImage &dest, const Eigen::MatrixX<RGBTuple> &src) {
    if (!dest.isGrayscale()) {
        for (int y = 0; y < src.rows(); ++y) {
            QRgb *line = reinterpret_cast<QRgb *>(dest.scanLine(y));
            for (int x = 0; x < src.cols(); ++x) {
                RGBTuple pixel = src(y, x);
                line[x] = qRgba(std::get<0>(pixel), std::get<1>(pixel), std::get<2>(pixel), 255);
            }
        }
    }
}

void ImageProcessorWrapper::invertColor(QImage &img) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Color::invert_color_rgb(imageV);
    // std::vector<std::vector<std::vector<uint>>> imageV = {{{0u, 0u, 0u}, {255u, 255u, 255u}},
    //                                                       {{255u, 255u, 255u}, {128u, 128u, 128u}}};
    matrixToRgbImage(img, imageV);
}

void ImageProcessorWrapper::mirror(QImage &img, const bool horizontal = true) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Transform::mirror(imageV, horizontal);
    matrixToRgbImage(img, imageV);
}

QImage ImageProcessorWrapper::rotateImage(QImage &img, double &angle) {
    // Eigen::MatrixX<RGBTuple> imageV = rgbImageToVector(img);
    // ImgProc::Transform::rotate_img(imageV, angle);
    // QImage rotated(imageV[0].size(), imageV.size(), img.format());
    // vectorToRgbImage(rotated, imageV);
    // return rotated;
    // std::vector<std::vector<uint>> imageV = imageToVector(img);
    // ImgProc::Transform::rotate_img_gray(imageV, angle);
    // QImage rotated(imageV[0].size(), imageV.size(), QImage::Format_ARGB32);
    // vectorToImage(rotated, imageV);
    // return rotated;
}

void ImageProcessorWrapper::toGray(QImage &img) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Color::to_gray(imageV);
    matrixToRgbImage(img, imageV);
}
