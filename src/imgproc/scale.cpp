#include "scale.h"

#include <functional>

#include <Eigen/Dense>

#include "../config/iptconfigmanager.h"

void ImgProc::Transform::scale_img_singlethreaded(const RGBMatrix &image,
                                                  RGBMatrix &scaled_image,
                                                  const Eigen::Matrix3d &transform_inv,
                                                  const int &start,
                                                  const int &end) {
    const int newRows = scaled_image.rows();
    const int newCols = scaled_image.cols();
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
                std::get<0>(scaled_image(x, y)) = std::round(interpolatedValue[0]);
                std::get<1>(scaled_image(x, y)) = std::round(interpolatedValue[1]);
                std::get<2>(scaled_image(x, y)) = std::round(interpolatedValue[2]);
            } else {
                std::get<0>(scaled_image(x, y)) = 0;
                std::get<1>(scaled_image(x, y)) = 0;
                std::get<2>(scaled_image(x, y)) = 0;
            }
        }
    }
}

ImgProc::RGBMatrix ImgProc::Transform::scale_img(RGBMatrix &image,
                                                 const double &sX,
                                                 const double &sY) {
    const int height = image.rows();
    const int width = image.cols();

    // Compute transformed image dimensions
    const int newHeight = std::ceil(height * sX);
    const int newWidth = std::ceil(width * sY);

    RGBMatrix scaled_img(newHeight, newWidth);

    Eigen::Matrix3d transform;
    transform << sX, 0, 0, 0, sY, 0, 0, 0, 1;

    Eigen::Matrix3d transform_inv = transform.inverse();

    unsigned int nthreads = IPTConfigManager::getInstance().getThreads();
    int rows_per_thread = newHeight / nthreads;
    std::thread *threads = new std::thread[nthreads];

    scale_img_singlethreaded(image, scaled_img, transform_inv, 0, newHeight);
    // for (int i = 0; i < nthreads; ++i) {
    //     int start = i * rows_per_thread;
    //     int end = (i < nthreads - 1) ? (i + 1) * rows_per_thread : /*new*/ height;
    //     threads[i] = std::thread(scale_img_singlethreaded,
    //                              std::ref(image),
    //                              std::ref(scaled_img),
    //                              std::ref(transform_inv),
    //                              std::ref(start),
    //                              std::ref(end));
    // }

    // for (int i = 0; i < nthreads; ++i) {
    //     threads[i].join();
    // }

    return scaled_img;
}
