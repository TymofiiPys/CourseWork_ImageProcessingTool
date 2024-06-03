#include "rotate.h"

#include <cmath>
#include <functional>
#include <mutex>

#include <Eigen/Dense>

#include "../config/iptconfigmanager.h"

std::mutex mutex;

void ImgProc::rotate_img_singlethreaded(const RGBMatrix &image,
                                        RGBMatrix &rotated_image,
                                        const Eigen::Matrix3d &transform_inv,
                                        const int start,
                                        const int end) {
    const int newRows = rotated_image.rows();
    const int newCols = rotated_image.cols();
    const int rows = image.rows();
    const int cols = image.cols();

    for (int x = start; x < end; ++x) {
        for (int y = 0; y < newCols; ++y) {
            Eigen::Vector3d newCoords(x, y, 1);

            Eigen::Vector3d oldCoords = transform_inv * newCoords;

            int x1 = std::floor(oldCoords(0));
            int x2 = std::ceil(oldCoords(0));
            int y1 = std::floor(oldCoords(1));
            int y2 = std::ceil(oldCoords(1));
            if (oldCoords(0) >= 0 && oldCoords(0) < rows - 1 && oldCoords(1) >= 0
                && oldCoords(1) < cols - 1) {
                double dx = oldCoords(0) - x1;
                double dy = oldCoords(1) - y1;
                double interpolatedValue[3];
                interpolatedValue[0] = (1 - dx) * (1 - dy) * std::get<0>(image(x1, y1))
                                       + dx * (1 - dy) * std::get<0>(image(x2, y1))
                                       + (1 - dx) * dy * std::get<0>(image(x1, y2))
                                       + dx * dy * std::get<0>(image(x2, y2));
                interpolatedValue[1] = (1 - dx) * (1 - dy) * std::get<1>(image(x1, y1))
                                       + dx * (1 - dy) * std::get<1>(image(x2, y1))
                                       + (1 - dx) * dy * std::get<1>(image(x1, y2))
                                       + dx * dy * std::get<1>(image(x2, y2));
                interpolatedValue[2] = (1 - dx) * (1 - dy) * std::get<2>(image(x1, y1))
                                       + dx * (1 - dy) * std::get<2>(image(x2, y1))
                                       + (1 - dx) * dy * std::get<2>(image(x1, y2))
                                       + dx * dy * std::get<2>(image(x2, y2));
                mutex.lock();
                std::get<0>(rotated_image(x, y)) = std::round(interpolatedValue[0]);
                std::get<1>(rotated_image(x, y)) = std::round(interpolatedValue[1]);
                std::get<2>(rotated_image(x, y)) = std::round(interpolatedValue[2]);
                mutex.unlock();
            } else {
                mutex.lock();
                std::get<0>(rotated_image(x, y)) = 0;
                std::get<1>(rotated_image(x, y)) = 0;
                std::get<2>(rotated_image(x, y)) = 0;
                mutex.unlock();
            }
        }
    }
}

ImgProc::RGBMatrix ImgProc::Transform::rotate_img(RGBMatrix &rgb_image, double &angle) {
    double rads = angle * M_PI / 180;

    const int height = rgb_image.rows();
    const int width = rgb_image.cols();

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
    const int newCenterX = newWidth / 2.0;
    const int newCenterY = newHeight / 2.0;

    RGBMatrix rotated_img(newHeight, newWidth);

    Eigen::Matrix3d rotation_matrix;
    rotation_matrix << cosTheta, -sinTheta, 0, sinTheta, cosTheta, 0, 0, 0, 1;
    Eigen::Matrix3d translation_before;
    translation_before << 1, 0, -centerX, 0, 1, -centerY, 0, 0, 1;
    Eigen::Matrix3d translation_after;
    translation_after << 1, 0, newCenterX, 0, 1, newCenterY, 0, 0, 1;
    Eigen::Matrix3d total_transform = translation_after * rotation_matrix * translation_before;
    Eigen::Matrix3d transform_inv = total_transform.inverse();

    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    int rows_per_thread = newHeight / nthreads;
    std::thread *threads = new std::thread[nthreads];

    // rotate_img_singlethreaded(rgb_image, rotated_img, transform_inv, 0, newHeight);
    for (int i = 0; i < nthreads; ++i) {
        int start = i * rows_per_thread;
        int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : newHeight;
        threads[i] = std::thread(rotate_img_singlethreaded,
                                 std::ref(rgb_image),
                                 std::ref(rotated_img),
                                 std::ref(transform_inv),
                                 start,
                                 end);
    }

    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }

    return rotated_img;
}

// void ImgProc::rotate_img_singlethreaded_gray(const std::vector<std::vector<unsigned int>> &image,
//                                              std::vector<std::vector<unsigned int>> &rotated_image,
//                                              const Eigen::Matrix3d &transform_inverse,
//                                              const int &start,
//                                              const int &end) {
//     // Get image dimensions
//     const int rows = rotated_image.size();
//     const int cols = rotated_image[0].size();
//     // const int newWidth = rotated_image[0].size();

//     // Compute center of rotation
//     const double centerY = cols / 2.0;
//     const double centerX = (end + start) / 2.0;

//     // Iterate over the rows assigned to this thread
//     for (int x = start; x < end; ++x) {
//         for (int y = 0; y < cols; ++y) {
//             // Map current pixel back to original image coordinates
//             Eigen::Vector3d target(x - centerX, y - centerY, 1);
//             Eigen::Vector3d source = transform_inverse * target;
//             double originalX = source(0) + centerX;
//             double originalY = source(1) + centerY;

//             if (originalX >= 0 && originalX < rows && originalY >= 0 && originalY < cols) {
//                 // int x1 = std::floor(originalX);
//                 // int x2 = std::ceil(originalX);
//                 // int y1 = std::floor(originalY);
//                 // int y2 = std::ceil(originalY);

//                 // double dx = originalX - x1;
//                 // double dy = originalY - y1;

//                 // for (int c = 0; c < 3; ++c) {
//                 // double interpolatedValue = (1 - dx) * (1 - dy) * image[x1][y1][c]
//                 //                            + dx * (1 - dy) * image[x2][y1][c]
//                 //                            + (1 - dx) * dy * image[x1][y2][c]
//                 //                            + dx * dy * image[x2][y2][c];
//                 // rotated_image[x][y][c] = round(interpolatedValue);
//                 rotated_image[x][y] = bicubicInterpolate(image, originalX, originalY);
//                 // }
//             }
//         }
//         if (x % 10 == 0)
//             qDebug() << "row " << x;
//     }
// }

// void ImgProc::rotate_img_gray(std::vector<std::vector<unsigned int>> &image, double &angle) {
//     double rads = angle * M_PI / 180;

//     const int &height = image.size();
//     const int &width = image[0].size();

//     // Compute transformed image dimensions
//     double sinTheta = std::sin(rads);
//     if (sinTheta < 0.000000001)
//         sinTheta = 0;
//     double cosTheta = std::cos(rads);
//     if (cosTheta < 0.000000001)
//         cosTheta = 0;
//     // const int newWidth = std::ceil(std::abs(width * cosTheta) + std::abs(height * sinTheta));
//     // const int newHeight = std::ceil(std::abs(width * sinTheta) + std::abs(height * cosTheta));

//     const int centerX = width / 2.0;
//     const int centerY = height / 2.0;

//     // std::vector<std::vector<std::vector<unsigned int>>>
//     //     rotated_img(newHeight,
//     //                 std::vector<std::vector<unsigned int>>(newWidth, std::vector<unsigned int>(3)));
//     std::vector<std::vector<unsigned int>> rotated_img(height, std::vector<unsigned int>(width));

//     Eigen::Matrix3d rotation_matrix;
//     rotation_matrix << cosTheta, -sinTheta, 0, sinTheta, cosTheta, 0, 0, 0, 1;

//     rotate_img_singlethreaded_gray(image,
//                                    rotated_img,
//                                    rotation_matrix.inverse(),
//                                    0,
//                                    rotated_img.size());

//     // unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
//     // int rows_per_thread = rotated_img.size() / nthreads;
//     // std::thread *threads = new std::thread[nthreads];

//     // for (int i = 0; i < nthreads; ++i) {
//     //     int start = i * rows_per_thread;
//     //     int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : rgb_image.size();
//     //     threads[i] = std::thread(rotate_img_singlethreaded,
//     //                              std::ref(rgb_image),
//     //                              ref(rotated_img),
//     //                              angle,
//     //                              start,
//     //                              end);
//     // }

//     // for (int i = 0; i < nthreads; ++i) {
//     //     threads[i].join();
//     // }

//     image = rotated_img;
// }
