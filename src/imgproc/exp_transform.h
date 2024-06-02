#ifndef EXP_TRANSFORM_H
#define EXP_TRANSFORM_H

#include "aliases.h"
namespace ImgProc {

inline namespace Color {

void exp_transform_singlethreaded(RGBMatrix &image,
                                  const int &start,
                                  const int &end,
                                  const double &c,
                                  const bool &red,
                                  const bool &green,
                                  const bool &blue);

void exp_transform(
    RGBMatrix &mage, const double &c, const bool &red, const bool &green, const bool &blue);

} // namespace Color

} // namespace ImgProc

#endif // EXP_TRANSFORM_H
