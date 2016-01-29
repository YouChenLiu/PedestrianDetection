#ifndef _MY_HOG_H_
#define _MY_HOG_H_

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include "../DIPKernel/DIPKernel.h"
#include "../myExtractorBase.h"

class myHOG : public myExtractorBase {
public:
    class Feature : protected myFeatureBase {
    public:
        static const int HOG_WITHOUT_NORM = 10;
        static const int HOG_WITH_L1_NORM = 11;
        static const int HOG_WITH_L1_SQRT = 12;
        static const int HOG_WITH_L2_NORM = 13;
        static const int HOG_WITH_L2_SQRT = 14;
    };

private:
    cv::Mat m_mHorizontalGradientImage;
    cv::Mat m_mVerticalGradientImage;
    int m_iInterval;
    cv::Size2i m_BlockSize;
    int m_iType;
    const float m_fUnimportantValue = 1e-6f;
    const int m_iMagnifyingFactor = 100;

public:
    myHOG(const cv::Mat& mImage, int iType, cv::Size2i blockSize = cv::Size2i(8, 8), int iInterval = 20);
    virtual ~myHOG(void) override;

    void Describe(cv::Point2i Position, std::vector<float>& vfHogFeature) const override;

private:
    void Init(void);
    void DescribeCell(const cv::Point2i Position, std::vector<float>& vfHogFeature) const;
    void Normalize(std::vector<float>& vfFeature) const;
};

#endif