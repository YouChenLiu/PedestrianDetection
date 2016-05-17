/**
 * @file BBComputation.h some calculation method for bounding box
 */
#ifndef _BB_COMPUTATION_H_
#define _BB_COMPUTATION_H_

#include "common.h"

namespace Verifier {

/**
 * @brief calculate the overlap ratio
 *
 * @param lhs bounding box1
 * @param rhs bounding box2
 * @return overlap ratio
 * @retval NAN wrong bounding box
 */
inline
auto CalOverlap(const cv::Rect2i& lhs, const cv::Rect2i& rhs) {
  auto lhsArea = lhs.area();
  auto rhsArea = rhs.area();
  if (lhsArea <= 0 || rhsArea <= 0) {
    return NAN;
  }
  auto intersection = lhs & rhs;
  auto overlapArea = intersection.area();
  auto unionArea = lhsArea + rhsArea - overlapArea;
  return static_cast<float>(overlapArea) / unionArea;
}

} // namespace

#endif // !_BB_COMPUTATION_H_