#include "myLBPIndexer.h"

std::array<unsigned int, 256> myLBPIndexer::m_aiMappingIndex;

myLBPIndexer::myLBPIndexer(void) : myLBP(), myModelIndexerBase() {
  Init();
  unsigned int iMapping = 0;
  for (unsigned int i = 0; i < 256; ++i) {
    auto bUniform = IsUniform(Feature::LBP_8_1_UNIFORM, i);
    m_aiMappingIndex.at(i) = bUniform ? iMapping++ : 58;
  }
}

myLBPIndexer::~myLBPIndexer(void) {}

unsigned int myLBPIndexer::GetBinNumber(const cv::Mat& mImage,
                                        const cv::Rect2i Region) const {
  // calculate the average of center region
  auto fCenterAverage = cv::mean(mImage(Region))[0];

  // the image will be calculate the bin number
  cv::Mat mImg = cv::Mat::zeros(3, 3, CV_8UC1);

  for (int y = -1; y <= 1; ++y) {
    for (int x = -1; x <= 1; ++x) {
      // set the center to 0
      if ((x | y) == 0) {
        mImg.at<unsigned char>(1, 1) = 0;
        continue;
      }

      // calculate the block average
      cv::Point2i ptStart(Region.x + x * Region.size().width,
                          Region.y + y * Region.size().height);
      auto AvgRange = CalRegion(ptStart, Region.size());
      cv::Mat mSubImg = mImage(AvgRange);
      auto fAverage = cv::mean(mSubImg)[0];

      // set corresponding position to 0 if the avg. is less then center
      unsigned char cResult = (fAverage <= fCenterAverage) ? 0 : 255u;
      mImg.at<unsigned char>(y + 1, x + 1) = cResult;
    }
  }
  
  auto BinNumber = myLBP::GetBinNumber(mImg);

  return m_aiMappingIndex.at(BinNumber);
}

void myLBPIndexer::Init(void) {
  m_ImgSize = cv::Size2i(64, 128);
}

cv::Rect2i myLBPIndexer::CalRegion(const cv::Point2i ptStart, const cv::Size2i Size) const {
  auto newStart = ptStart;
  auto newSize = Size;
  if (ptStart.x < 0) {
    newStart.x = 0;
    newSize.width = ptStart.x + Size.width;
  }
  if (newStart.y < 0) {
    newStart.y = 0;
    newSize.height = ptStart.y + Size.height;
  }

  if (ptStart.x + Size.width > m_ImgSize.width) {
    newSize.width = m_ImgSize.width - ptStart.x;
  }
  if (ptStart.y + Size.height > m_ImgSize.height) {
    newSize.height = m_ImgSize.height - ptStart.y;
  }
  return cv::Rect2i(newStart, newSize);
}