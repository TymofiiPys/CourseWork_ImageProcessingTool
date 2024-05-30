#include "rotate.h"

#include <algorithm>
#include <cmath>
#include <functional>

#include <Eigen/Dense>

#include "../config/iptconfigmanager.h"

// Bicubic kernel function
float cubicInterpolate(float p0, float p1, float p2, float p3, float x) {
    return p1
           + 0.5 * x
                 * (p2 - p0
                    + x * (2.0 * p0 - 5.0 * p1 + 4.0 * p2 - p3 + x * (3.0 * (p1 - p2) + p3 - p0)));
}

// Get pixel value with boundary check
float getPixelValue(const std::vector<std::vector<std::vector<unsigned int>>> &img,
                    int x,
                    int y,
                    int ch) {
    x = std::clamp(x, 0, (int) img.size() - 1);
    y = std::clamp(y, 0, (int) img[0].size() - 1);
    return img[x][y][ch];
}

// Bicubic interpolation function
float bicubicInterpolate(const std::vector<std::vector<std::vector<unsigned int>>> &img,
                         float x,
                         float y,
                         int ch) {
    int xInt = static_cast<int>(x);
    int yInt = static_cast<int>(y);
    float xDiff = x - xInt;
    float yDiff = y - yInt;

    float patch[4][4];
    for (int i = -1; i <= 2; ++i) {
        for (int j = -1; j <= 2; ++j) {
            patch[i + 1][j + 1] = getPixelValue(img, xInt + i, yInt + j, ch);
        }
    }

    float colInterpolated[4];
    for (int i = 0; i < 4; ++i) {
        colInterpolated[i] = cubicInterpolate(patch[i][0],
                                              patch[i][1],
                                              patch[i][2],
                                              patch[i][3],
                                              xDiff);
    }
    return std::clamp(cubicInterpolate(colInterpolated[0],
                                       colInterpolated[1],
                                       colInterpolated[2],
                                       colInterpolated[3],
                                       yDiff),
                      0.0f,
                      255.0f);
}

// Get pixel value with boundary check
float getPixelValue(const std::vector<std::vector<unsigned int>> &img, int x, int y) {
    x = std::clamp(x, 0, (int) img.size() - 1);
    y = std::clamp(y, 0, (int) img[0].size() - 1);
    return img[x][y];
}

// Bicubic interpolation function
int bicubicInterpolate(const std::vector<std::vector<unsigned int>> &img, float x, float y) {
    int xInt = static_cast<int>(x);
    int yInt = static_cast<int>(y);
    float xDiff = x - xInt;
    float yDiff = y - yInt;

    float patch[4][4];
    for (int i = -1; i <= 2; ++i) {
        for (int j = -1; j <= 2; ++j) {
            patch[i + 1][j + 1] = getPixelValue(img, xInt + i, yInt + j);
        }
    }

    float colInterpolated[4];
    for (int i = 0; i < 4; ++i) {
        colInterpolated[i] = cubicInterpolate(patch[i][0],
                                              patch[i][1],
                                              patch[i][2],
                                              patch[i][3],
                                              xDiff);
    }
    return std::clamp(static_cast<int>(cubicInterpolate(colInterpolated[0],
                                                        colInterpolated[1],
                                                        colInterpolated[2],
                                                        colInterpolated[3],
                                                        yDiff)),
                      0,
                      255);
}

void ImgProc::rotate_img_singlethreaded(
    const std::vector<std::vector<std::vector<unsigned int>>> &image,
    std::vector<std::vector<std::vector<unsigned int>>> &rotated_image,
    const Eigen::Matrix3d &transform_inverse,
    const int &start,
    const int &end) {
    // Get image dimensions
    const int newRows = rotated_image.size();
    const int newCols = rotated_image[0].size();
    const int rows = image.size();
    const int cols = image[0].size();
    // const int newWidth = rotated_image[0].size();

    // Compute center of rotation
    const double centerY = cols / 2.0;
    const double centerX = (end + start) / 2.0;

    // Iterate over the rows assigned to this thread
    for (int x = start; x < end; ++x) {
        for (int y = 0; y < newCols; ++y) {
            // Map current pixel back to original image coordinates
            Eigen::Vector3d target(x - centerX, y - centerY, 1);
            Eigen::Vector3d source = transform_inverse * target;
            double originalX = source(0) + centerX;
            double originalY = source(1) + centerY;

            if (originalX >= 0 && originalX < rows && originalY >= 0 && originalY < cols) {
                int x1 = std::floor(originalX);
                int x2 = std::ceil(originalX);
                int y1 = std::floor(originalY);
                int y2 = std::ceil(originalY);

                double dx = originalX - x1;
                double dy = originalY - y1;

                for (int c = 0; c < 3; ++c) {
                    double interpolatedValue = (1 - dx) * (1 - dy) * image[x1][y1][c]
                                               + dx * (1 - dy) * image[x2][y1][c]
                                               + (1 - dx) * dy * image[x1][y2][c]
                                               + dx * dy * image[x2][y2][c];
                    rotated_image[x][y][c] = round(interpolatedValue);
                    // rotated_image[x][y][c] = bicubicInterpolate(image, originalX, originalY, c);
                }
            }
        }
        if (x % 10 == 0)
            qDebug() << "row " << x;
    }
}

void ImgProc::Transform::rotate_img(std::vector<std::vector<std::vector<unsigned int>>> &rgb_image,
                                    double &angle) {
    double rads = angle * M_PI / 180;

    const int &height = rgb_image.size();
    const int &width = rgb_image[0].size();

    // Compute transformed image dimensions
    double sinTheta = std::sin(rads);
    if (sinTheta < 0.000000001)
        sinTheta = 0;
    double cosTheta = std::cos(rads);
    if (cosTheta < 0.000000001)
        cosTheta = 0;
    const int newWidth = std::ceil(std::abs(width * cosTheta) + std::abs(height * sinTheta));
    const int newHeight = std::ceil(std::abs(width * sinTheta) + std::abs(height * cosTheta));

    const int centerX = width / 2.0;
    const int centerY = height / 2.0;

    std::vector<std::vector<std::vector<unsigned int>>>
        rotated_img(newHeight,
                    std::vector<std::vector<unsigned int>>(newWidth, std::vector<unsigned int>(3)));
    // std::vector<std::vector<std::vector<unsigned int>>>
    //     rotated_img(height,
    //                 std::vector<std::vector<unsigned int>>(width, std::vector<unsigned int>(3)));

    Eigen::Matrix3d rotation_matrix;
    rotation_matrix << cosTheta, -sinTheta, 0, sinTheta, cosTheta, 0, 0, 0, 1;

    rotate_img_singlethreaded(rgb_image,
                              rotated_img,
                              rotation_matrix.inverse(),
                              0,
                              rotated_img.size());

    // unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    // int rows_per_thread = rotated_img.size() / nthreads;
    // std::thread *threads = new std::thread[nthreads];

    // for (int i = 0; i < nthreads; ++i) {
    //     int start = i * rows_per_thread;
    //     int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : rgb_image.size();
    //     threads[i] = std::thread(rotate_img_singlethreaded,
    //                              std::ref(rgb_image),
    //                              ref(rotated_img),
    //                              angle,
    //                              start,
    //                              end);
    // }

    // for (int i = 0; i < nthreads; ++i) {
    //     threads[i].join();
    // }

    rgb_image = rotated_img;
}

void ImgProc::rotate_img_singlethreaded_gray(const std::vector<std::vector<unsigned int>> &image,
                                             std::vector<std::vector<unsigned int>> &rotated_image,
                                             const Eigen::Matrix3d &transform_inverse,
                                             const int &start,
                                             const int &end) {
    // Get image dimensions
    const int rows = rotated_image.size();
    const int cols = rotated_image[0].size();
    // const int newWidth = rotated_image[0].size();

    // Compute center of rotation
    const double centerY = cols / 2.0;
    const double centerX = (end + start) / 2.0;

    // Iterate over the rows assigned to this thread
    for (int x = start; x < end; ++x) {
        for (int y = 0; y < cols; ++y) {
            // Map current pixel back to original image coordinates
            Eigen::Vector3d target(x - centerX, y - centerY, 1);
            Eigen::Vector3d source = transform_inverse * target;
            double originalX = source(0) + centerX;
            double originalY = source(1) + centerY;

            if (originalX >= 0 && originalX < rows && originalY >= 0 && originalY < cols) {
                // int x1 = std::floor(originalX);
                // int x2 = std::ceil(originalX);
                // int y1 = std::floor(originalY);
                // int y2 = std::ceil(originalY);

                // double dx = originalX - x1;
                // double dy = originalY - y1;

                // for (int c = 0; c < 3; ++c) {
                // double interpolatedValue = (1 - dx) * (1 - dy) * image[x1][y1][c]
                //                            + dx * (1 - dy) * image[x2][y1][c]
                //                            + (1 - dx) * dy * image[x1][y2][c]
                //                            + dx * dy * image[x2][y2][c];
                // rotated_image[x][y][c] = round(interpolatedValue);
                rotated_image[x][y] = bicubicInterpolate(image, originalX, originalY);
                // }
            }
        }
        if (x % 10 == 0)
            qDebug() << "row " << x;
    }
}

void ImgProc::rotate_img_gray(std::vector<std::vector<unsigned int>> &image, double &angle) {
    double rads = angle * M_PI / 180;

    const int &height = image.size();
    const int &width = image[0].size();

    // Compute transformed image dimensions
    double sinTheta = std::sin(rads);
    if (sinTheta < 0.000000001)
        sinTheta = 0;
    double cosTheta = std::cos(rads);
    if (cosTheta < 0.000000001)
        cosTheta = 0;
    // const int newWidth = std::ceil(std::abs(width * cosTheta) + std::abs(height * sinTheta));
    // const int newHeight = std::ceil(std::abs(width * sinTheta) + std::abs(height * cosTheta));

    const int centerX = width / 2.0;
    const int centerY = height / 2.0;

    // std::vector<std::vector<std::vector<unsigned int>>>
    //     rotated_img(newHeight,
    //                 std::vector<std::vector<unsigned int>>(newWidth, std::vector<unsigned int>(3)));
    std::vector<std::vector<unsigned int>> rotated_img(height, std::vector<unsigned int>(width));

    Eigen::Matrix3d rotation_matrix;
    rotation_matrix << cosTheta, -sinTheta, 0, sinTheta, cosTheta, 0, 0, 0, 1;

    rotate_img_singlethreaded_gray(image,
                                   rotated_img,
                                   rotation_matrix.inverse(),
                                   0,
                                   rotated_img.size());

    // unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    // int rows_per_thread = rotated_img.size() / nthreads;
    // std::thread *threads = new std::thread[nthreads];

    // for (int i = 0; i < nthreads; ++i) {
    //     int start = i * rows_per_thread;
    //     int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : rgb_image.size();
    //     threads[i] = std::thread(rotate_img_singlethreaded,
    //                              std::ref(rgb_image),
    //                              ref(rotated_img),
    //                              angle,
    //                              start,
    //                              end);
    // }

    // for (int i = 0; i < nthreads; ++i) {
    //     threads[i].join();
    // }

    image = rotated_img;
}
