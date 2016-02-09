#include "myBlockBasedExtractor.h"

myBlockBasedExtractor::myBlockBasedExtractor(void) : myExtractorBase() {
    SetBlockSize(cv::Size2i(8, 8));
}

myBlockBasedExtractor::myBlockBasedExtractor(const cv::Mat & mImage,
                                             const cv::Size2i BlockSize) :
    myExtractorBase(mImage) {
    m_BlockSize = BlockSize;
}

myBlockBasedExtractor::~myBlockBasedExtractor(void) {}