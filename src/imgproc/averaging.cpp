#include "averaging.h"

void ImgProc::weighted_averaging(RGBMatrix &img) {
    RGBMatrix newimg(img.rows(), img.cols());
    int halfW = matrix.rows() / 2;
    int halfH = matrix.rows() / 2;
    for (int x = 0; x < newimg.rows(); ++x) {
        for (int y = 0; y < newimg.cols(); ++y) {
            double weightR = 0;
            double weightG = 0;
            double weightB = 0;
            double denum = 0;
            for (int i = -halfW; i <= halfH; ++i) {
                if (x + i >= img.rows() || x + i < 0)
                    continue;
                for (int j = -halfH; j <= halfH; ++j) {
                    if (y + j >= img.cols() || y + j < 0)
                        continue;
                    weightR += matrix(i + halfW, j + halfH) * std::get<0>(img(x + i, y + j));
                    weightG += matrix(i + halfW, j + halfH) * std::get<1>(img(x + i, y + j));
                    weightB += matrix(i + halfW, j + halfH) * std::get<2>(img(x + i, y + j));
                    denum += matrix(i + halfW, j + halfH);
                }
            }
            std::get<0>(newimg(x, y)) = weightR / denum;
            std::get<1>(newimg(x, y)) = weightG / denum;
            std::get<2>(newimg(x, y)) = weightB / denum;
        }
    }
    img = newimg;
}
