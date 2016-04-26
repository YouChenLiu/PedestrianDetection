#ifndef _MY_SCANNER_H_
#define _MY_SCANNER_H_

#include "../common.h"

class myScanner {
private:
  cv::Size2i m_ImgSize;
  cv::Size2i m_BlockSize;

public:
  myScanner(const cv::Size2i ImgSize, const cv::Size2i BlockSize) {
    m_ImgSize = ImgSize;
    m_BlockSize = BlockSize;
  }

  ~myScanner(void) {}
  
  void CalRect(std::vector<cv::Rect2i>& vRect,
               cv::Point2i ptLeftTop,
               cv::Point2i ptRightButtom,
               cv::Point2i ptInterval = cv::Point2i(0, 0)) {
    if (ptInterval == cv::Point2i(0, 0)) {
      ptInterval = m_BlockSize;
    }

    int ThresholdY = m_ImgSize.height - m_BlockSize.height;
    int ThresholdX = m_ImgSize.width - m_BlockSize.width;
    for (int y = ptLeftTop.y; y <= ThresholdY; y += ptInterval.y) {
      for (int x = ptLeftTop.x; x <= ThresholdX; x += ptInterval.x) {
        if (x + m_BlockSize.width > ptRightButtom.x) {
          break;
        }
        vRect.push_back(cv::Rect2i(x, y, m_BlockSize.width, m_BlockSize.height));
      }

      if (y + m_BlockSize.height > ptRightButtom.y) {
        continue;
      }
    }
  }

};  //class

#endif // !_MY_SCANNER_H_