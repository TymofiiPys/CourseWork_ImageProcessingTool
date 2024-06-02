#ifndef LOG_TRANSFORM_H
#define LOG_TRANSFORM_H

#include "aliases.h"

namespace ImgProc {

inline namespace Color {

void log_transform_singlethreaded(RGBMatrix &image,
                                  const int &start,
                                  const int &end,
                                  const double &c,
                                  const bool &red,
                                  const bool &green,
                                  const bool &blue);

void log_transform(
    RGBMatrix &mage, const double &c, const bool &red, const bool &green, const bool &blue);

} // namespace Color

} // namespace ImgProc

#endif // LOG_TRANSFORM_H
