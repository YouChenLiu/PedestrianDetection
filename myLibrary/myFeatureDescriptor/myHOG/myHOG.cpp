#include "myHOG.h"
#include "DIPKernel/DIPKernel.h"

namespace Descriptor {

static const std::array<DIPKernel, 2> m_aoHogMask = {
    DIPKernel(DIPKernel::Types::SIMPLE_X), DIPKernel(DIPKernel::Types::SIMPLE_Y)
};

myHOG::myHOG(const cv::Mat& mImage, int iType, cv::Size2i BlockSize,
             int iInterval) : myBlockDescriptorBase(cv::Mat(), BlockSize) {
  Init();
  if (!mImage.empty()) {
    m_mHorizontalGradientImage = mImage * m_aoHogMask.at(0);
    m_mVerticalGradientImage = mImage * m_aoHogMask.at(1);
  }
  m_iInterval = iInterval;
  m_iFeatureType = iType;
}

myHOG::~myHOG(void) {}

void myHOG::Init(void) {
  m_mHorizontalGradientImage = m_mVerticalGradientImage = cv::Mat();
  m_iInterval = 0;
}

void myHOG::Describe(cv::Point2i Position, std::vector<float>& vfHogFeature) const {
  cv::Size2i CellSize;

  switch (m_iFeatureType & ~NORMALIZATION_FLAG & ~NORM_MASK) {
  case Feature::HOG_SINGLE_CELL:
    CellSize = cv::Size2i(m_BlockSize.width, m_BlockSize.height);
    DescribeCell(Position, CellSize, vfHogFeature);
    break;
  case Feature::HOG_STANDARD:
    CellSize = cv::Size2i(m_BlockSize.width / 2, m_BlockSize.height / 2);
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 2; ++j) {
        cv::Point2i RoiLocation = Position + cv::Point2i(j * CellSize.width, i * CellSize.height);
        DescribeCell(RoiLocation, CellSize, vfHogFeature);
      }
    }
    break;
  default:
    std::cout << "unknow HOG type" << std::endl;
    break;
  }

}

void myHOG::SetImage(const cv::Mat& mImage) {
  m_mHorizontalGradientImage = mImage * m_aoHogMask.at(0);
  m_mVerticalGradientImage = mImage * m_aoHogMask.at(1);
}

void myHOG::DescribeCell(const cv::Point2i Position,
                         cv::Size2i CellSize,
                         std::vector<float>& vfHogFeature) const {
  std::vector<float> vfBins(180 / m_iInterval, 0.0f);
  cv::Mat mHorizontalRoi = cv::Mat(m_mHorizontalGradientImage, cv::Rect(Position, CellSize));
  cv::Mat mVerticalRoi = cv::Mat(m_mVerticalGradientImage, cv::Rect(Position, CellSize));

  for (int y = 0; y < CellSize.height; ++y) {
    for (int x = 0; x < CellSize.width; ++x) {
      float fOrientation = atan2f(mVerticalRoi.at<short>(y, x), mHorizontalRoi.at<short>(y, x)) * 180.0f / static_cast<float>(CV_PI);
      float fMagnitude = sqrtf(static_cast<float>(mVerticalRoi.at<short>(y, x) * mVerticalRoi.at<short>(y, x) +
                                                  mHorizontalRoi.at<short>(y, x) * mHorizontalRoi.at<short>(y, x)));
      if (fOrientation < 0.0f) {
        fOrientation += 180.0f;
      } else if (fOrientation > 180.0f) {
        fOrientation -= 180.0f;
      }

      auto target = static_cast<size_t>(fOrientation / m_iInterval);
      if (target == vfBins.size()) {
        target = vfBins.size() - 1;
      }
      vfBins.at(target) += fMagnitude;
    }
  }

  for (auto bin : vfBins) {
    vfHogFeature.push_back(bin);
  }
}

}