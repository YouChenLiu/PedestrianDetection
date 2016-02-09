#ifndef _MY_FEATURE_EXTRACTOR_H_
#define _MY_FEATURE_EXTRACTOR_H_

#include <iostream>
#include <memory>
#include "../myFeatureExtractor/myExtractorBase.h"
#include "myHOG/myHOG.h"
#include "myLBP/myLBP.h"

#ifndef NDEBUG
#   include <iomanip>
#   include <fstream>
#endif

class myFeatureExtractor : public myExtractorBase {
public:
    class Features : public myHOG::Feature, public myLBP::Feature {};
private:
    cv::Size2i m_BlockSize;
    cv::Mat m_mImage;
    std::vector<std::unique_ptr<myExtractorBase>> m_vpoUsedExtractor;

public:
    myFeatureExtractor(const cv::Mat& mImage, cv::Size2i BlockSize = cv::Size2i(8, 8));
    myFeatureExtractor(IplImage* pImage, CvSize BlockSize = cvSize(8, 8));
    ~myFeatureExtractor(void);

    void Describe(cv::Point2i Position, std::vector<float>& vfFeature) const;
    void Describe(int x, int y, std::vector<float>& vfFeature) const { Describe(cv::Point2i(x, y), vfFeature); }
    
    void EnableFeature(int iFeature);
    void SetBlockSize(cv::Size2i Size) { m_BlockSize = Size; }
    void SetBlockSize(int Width, int Height) { SetBlockSize(cv::Size2i(Width, Height)); }

private:
    void Init(void);
    
};

#endif