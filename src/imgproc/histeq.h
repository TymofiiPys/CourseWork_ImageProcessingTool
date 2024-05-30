#ifndef HISTEQ_H
#define HISTEQ_H
#include "aliases.h"

namespace ImgProc {

inline namespace Color {

using RGBHistogram = std::array<std::array<unsigned int, 256>, 3>;

void hist_eq_singlethreaded(RGBMatrix &image,
                            const RGBHistogram &hist,
                            const int &start,
                            const int &end);
void hist_eq(RGBMatrix &image);

} // namespace Color

} // namespace ImgProc
#endif // HISTEQ_H
