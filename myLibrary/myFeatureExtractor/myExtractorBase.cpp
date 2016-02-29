#include "myExtractorBase.h"

myExtractorBase::myExtractorBase(void) {
    m_mImage = cv::Mat();
}

myExtractorBase::myExtractorBase(const cv::Mat& mImage) {
    SetImage(mImage);
}

myExtractorBase::~myExtractorBase(void) {}
