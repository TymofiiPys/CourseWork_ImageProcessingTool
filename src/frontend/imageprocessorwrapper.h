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
    static void mirror(QImage &, const bool horizontal);
    static QImage rotateImage(QImage &, double &angle);
};

#endif // IMAGEPROCESSORWRAPPER_H
