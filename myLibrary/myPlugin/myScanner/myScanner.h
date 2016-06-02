/**
 * @file myScanner.h
 * @brief myScanner class definition
 */
#ifndef _MY_SCANNER_H_
#define _MY_SCANNER_H_

#include "common.h"

namespace Plugin {

/**
 * @brief region calculator
 */
class myScanner {
private:
  cv::Point2i m_ptLT;
  cv::Point2i m_ptRB;

public:
  /**
   * @brief
   *
   * @param ptLeftTop
   * @param ptRightButtom
   */
  myScanner(const cv::Point2i ptLeftTop, const cv::Point2i ptRightButtom) {
    m_ptLT = ptLeftTop;
    m_ptRB = ptRightButtom;
  }

  ~myScanner(void) {}

  /**
   * @brief 
   *
   * @param vRect
   * @param BlockSize
   * @param ptStep
   */
  void CalRect(std::vector<cv::Rect2i>& vRect,
               cv::Size2i BlockSize,
               cv::Point2i ptStep) {
    for (int y = m_ptLT.y; y < m_ptRB.y; y += ptStep.y) {
      if (y + BlockSize.height > m_ptRB.y) {
        break;
      }
      for (int x = m_ptLT.x; x < m_ptRB.x; x += ptStep.x) {
        if (x + BlockSize.width > m_ptRB.x) {
          break;
        }
        vRect.push_back(cv::Rect2i(x, y, BlockSize.width, BlockSize.height));
      }
    }
  }

};  //class

} // namespace
#endif // !_MY_SCANNER_H_