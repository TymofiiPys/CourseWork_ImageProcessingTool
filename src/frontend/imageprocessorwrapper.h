#ifndef IMAGEPROCESSORWRAPPER_H
#define IMAGEPROCESSORWRAPPER_H

#include <vector>

#include <Eigen/Dense>
#include <QImage>

using RGBTuple = std::tuple<uint, uint, uint>;

class ImageProcessorWrapper {
  private:
    static Eigen::MatrixX<RGBTuple> rgbImageToMatrix(const QImage &);
    static std::vector<std::vector<uint>> imageToVector(const QImage &);
    static void vectorToImage(QImage &dest, const std::vector<std::vector<uint>> &src);
    static void matrixToRgbImage(QImage &dest, const Eigen::MatrixX<RGBTuple> &);

  public:
    static void invertColor(QImage &);
    static void toGray(QImage &);
    static void histEq(QImage &, const bool red, const bool green, const bool blue);
    static void logTransform(
        QImage &, const double c, const bool red, const bool green, const bool blue);
    static void expTransform(
        QImage &, const double c, const bool red, const bool green, const bool blue);
    static void mirror(QImage &, const bool horizontal);
    static QImage rotateImage(QImage &, double &angle);
    static QImage scaleImage(QImage &, double sX, double sY);
    static void boxFilter(QImage &);
    static void gaussBlur(QImage &, const int radius);
    static void laplacian(QImage &);
};

#endif // IMAGEPROCESSORWRAPPER_H
