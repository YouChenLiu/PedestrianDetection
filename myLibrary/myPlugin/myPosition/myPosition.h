/**
 * @file myPosition.h
 * @brief myPosition class definition
 */

#ifndef _MY_POSITION_H_
#define _MY_POSITION_H_

#include "common.h"

namespace Plugin {

/**
 * @brief position generator
 */
class myPosition {
private:
  cv::Point2i m_ptStart;
  cv::Point2i m_ptEnd;

public:
  /**
   * @brief create object by boundary
   *
   * @param ptStart left-top point
   * @param ptEnd right-bottom point
   */
  myPosition(cv::Point2i ptStart, cv::Point2i ptEnd) {
    m_ptStart = ptStart;
    m_ptEnd = ptEnd;
  }

  /**
   * @brief calculate points in boundary
   *
   * @param vptPos vector for storing positions
   * @param Step interval for moving to next point
   */
  void CalPositions(std::vector<cv::Point2i>& vptPos,const cv::Point2i& Step) {
    vptPos.clear();
    for (int y = m_ptStart.y; y < m_ptEnd.y; y += Step.y) {
      for (int x = m_ptStart.x; x < m_ptEnd.x; x += Step.x) {
        vptPos.push_back(cv::Point2i(x, y));
      }
    }
  }
};

}
#endif // !_MY_POSITION_H_