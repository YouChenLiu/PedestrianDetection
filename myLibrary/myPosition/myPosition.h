/**
 * @file myPosition.h
 * @brief myPosition class definition
 */

#ifndef _MY_POSITION_H_
#define _MY_POSITION_H_

#include "../common.h"

namespace Plugin {

class myPosition {
private:
  cv::Point2i m_ptStart;
  cv::Point2i m_ptEnd;

public:
  myPosition(cv::Point2i ptStart, cv::Point2i ptEnd) {
    m_ptStart = ptStart;
    m_ptEnd = ptEnd;
  }

  void CalPositions(std::vector<cv::Point2i>& vptPos, cv::Point2i Step) {
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