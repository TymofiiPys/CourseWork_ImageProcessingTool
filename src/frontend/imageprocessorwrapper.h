#ifndef IMAGEPROCESSORWRAPPER_H
#define IMAGEPROCESSORWRAPPER_H

#include <vector>

#include <QImage>

class ImageProcessorWrapper {
  private:
    static std::vector<std::vector<std::vector<uchar>>> rgbImageToVector(const QImage &);
    static std::vector<std::vector<uint>> imageToVector(const QImage &);
    static void vectorToImage(QImage &dest, const std::vector<std::vector<uint>> &src);
    static void vectorToRgbImage(QImage &dest,
                                 const std::vector<std::vector<std::vector<uchar>>> &src);

  public:
    static void invertColor(QImage &);
};

#endif // IMAGEPROCESSORWRAPPER_H
