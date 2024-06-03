#include "imageprocessorwrapper.h"

#include <QColor>
#include <QDebug>

#include "../imgproc/averaging.h"
#include "../imgproc/exp_transform.h"
#include "../imgproc/histeq.h"
#include "../imgproc/invert.h"
#include "../imgproc/log_transform.h"
#include "../imgproc/mirror.h"
#include "../imgproc/rotate.h"
#include "../imgproc/scale.h"
#include "../imgproc/togray.h"

Eigen::MatrixX<RGBTuple> ImageProcessorWrapper::rgbImageToMatrix(const QImage &from) {
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

std::vector<std::vector<uint>> ImageProcessorWrapper::imageToVector(const QImage &from) {
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
    for (int y = 0; y < src.rows(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(dest.scanLine(y));
        for (int x = 0; x < src.cols(); ++x) {
            RGBTuple pixel = src(y, x);
            line[x] = qRgba(std::get<0>(pixel), std::get<1>(pixel), std::get<2>(pixel), 255);
        }
    }
}

void ImageProcessorWrapper::invertColor(QImage &img) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Color::invert_color_rgb(imageV);
    matrixToRgbImage(img, imageV);
}

void ImageProcessorWrapper::mirror(QImage &img, const bool horizontal = true) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Transform::mirror(imageV, horizontal);
    matrixToRgbImage(img, imageV);
}

QImage ImageProcessorWrapper::rotateImage(QImage &img, double &angle) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    Eigen::MatrixX<RGBTuple> ret = ImgProc::Transform::rotate_img(imageV, angle);
    qDebug() << "image " << std::get<0>(imageV(0, 0));
    qDebug() << "ret image " << std::get<0>(ret(3900, 0));
    QImage rotated(ret.cols(), ret.rows(), QImage::Format_ARGB32);
    matrixToRgbImage(rotated, ret);
    return rotated;
}

QImage ImageProcessorWrapper::scaleImage(QImage &img, double sX, double sY) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    Eigen::MatrixX<RGBTuple> ret = ImgProc::Transform::scale_img(imageV, sX, sY);
    QImage scaled(ret.cols(), ret.rows(), QImage::Format_ARGB32);
    matrixToRgbImage(scaled, ret);
    return scaled;
}

void ImageProcessorWrapper::toGray(QImage &img) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Color::to_gray(imageV);
    matrixToRgbImage(img, imageV);
}

void ImageProcessorWrapper::histEq(QImage &img, const bool red, const bool green, const bool blue) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Color::hist_eq(imageV, red, green, blue);
    matrixToRgbImage(img, imageV);
}

void ImageProcessorWrapper::logTransform(
    QImage &img, const double c, const bool red, const bool green, const bool blue) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Color::log_transform(imageV, c, red, green, blue);
    matrixToRgbImage(img, imageV);
}

void ImageProcessorWrapper::expTransform(
    QImage &img, const double c, const bool red, const bool green, const bool blue) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Color::exp_transform(imageV, c, red, green, blue);
    matrixToRgbImage(img, imageV);
}

void ImageProcessorWrapper::weightedAverage(QImage &img) {
    Eigen::MatrixX<RGBTuple> imageV = rgbImageToMatrix(img);
    ImgProc::Filter::weighted_averaging(imageV);
    matrixToRgbImage(img, imageV);
}
