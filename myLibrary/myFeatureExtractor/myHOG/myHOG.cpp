#include "myHOG.h"

const float myHOG::m_fUnimportantValue = 1e-6f;
static const std::array<DIPKernel, 2> m_aoHogMask = {
    DIPKernel(DIPKernel::Types::SIMPLE_X), DIPKernel(DIPKernel::Types::SIMPLE_Y)
};

myHOG::myHOG(const cv::Mat& mImage, int iType, cv::Size2i BlockSize,
             int iInterval) : myBlockBasedExtractor(mImage, BlockSize) {
    Init();
    m_mHorizontalGradientImage = mImage * m_aoHogMask.at(0);
    m_mVerticalGradientImage = mImage * m_aoHogMask.at(1);
    m_iInterval = iInterval;
    m_iType = iType;
}

myHOG::~myHOG(void) {}

void myHOG::Init(void) {
    m_mHorizontalGradientImage = m_mVerticalGradientImage = cv::Mat();
    m_iInterval = 0;
}

void myHOG::Normalize(std::vector<float>& vfFeature) const {
    float fNormFactor = 0.0f;
    switch (m_iType) {
    case myHOG::Feature::HOG_WITH_L1_NORM:
        fNormFactor = m_fUnimportantValue;
        for (auto fFeature : vfFeature) {
            fNormFactor += fFeature;
        }

        for (auto& fFeature : vfFeature) {
            fFeature = fFeature / fNormFactor * m_iMagnifyingFactor;
        }
        break;
    case myHOG::Feature::HOG_WITH_L1_SQRT:
        fNormFactor = m_fUnimportantValue;
        for (auto fFeature : vfFeature) {
            fNormFactor += fFeature;
        }

        for (auto& fFeature : vfFeature) {
            fFeature = cv::sqrt(fFeature / fNormFactor) * m_iMagnifyingFactor;
        }
        break;
    case myHOG::Feature::HOG_WITH_L2_NORM:
        fNormFactor = m_fUnimportantValue * m_fUnimportantValue;
        for (auto fFeature : vfFeature) {
            fNormFactor += fFeature * fFeature;
        }
        fNormFactor = cv::sqrt(fNormFactor);

        for (auto& fFeature : vfFeature) {
            fFeature = fFeature / fNormFactor * m_iMagnifyingFactor;
        }
        break;
    default:
        std::cout << "not support normalization method" << std::endl;
        break;
    }
}

void myHOG::Describe(cv::Point2i Position, std::vector<float>& vfHogFeature) const {
    cv::Size2i CellSize = cv::Size2i(m_BlockSize.width / 2, m_BlockSize.height / 2);
    
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            cv::Point2i RoiLocation = Position + cv::Point2i(j * CellSize.width, i * CellSize.height);
            DescribeCell(RoiLocation, vfHogFeature);
        }
    }

    if (m_iType != myHOG::Feature::HOG_WITHOUT_NORM) {
        myHOG::Normalize(vfHogFeature);
    }
}

void myHOG::DescribeCell(const cv::Point2i Position, std::vector<float>& vfHogFeature) const {
    std::vector<float> vfBins(180 / m_iInterval, 0.0f);
    cv::Size2i CellSize = cv::Size2i(m_BlockSize.width / 2, m_BlockSize.height / 2);
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