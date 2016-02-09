#include "myLBPIndexer.h"

myLBPIndexer::myLBPIndexer(void) : myLBP(), myModelIndexerBase() {}

myLBPIndexer::myLBPIndexer(const cv::Mat & mImage) :
    myLBP(mImage, myLBP::Feature::LBP_8_1_UNIFORM) {}

myLBPIndexer::~myLBPIndexer(void) {}

unsigned int myLBPIndexer::GetBinNumber(const cv::Point2i Position) const {
    unsigned int iBinNumber = 0;
    
    cv::Mat mImg = cv::Mat::zeros(3, 3, CV_32FC1);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            cv::Rect2i AvgRange(cv::Point2i(Position.x + x * m_BlockSize.width,
                                            Position.y + y * m_BlockSize.height),
                                m_BlockSize);
            cv::Mat mSubImg = m_mImage(AvgRange);
            mImg.at<cv::Scalar>(y + 1, x + 1) = cv::mean(mSubImg);
        }
    }

    iBinNumber = myLBP::GetBinNumber(mImg);
    if (!IsUniform(iBinNumber, 8)) {
        iBinNumber = GetNumOfBins() - 1;
    }
    return myLBP::GetBinNumber(mImg);
}

void myLBPIndexer::Init(void) {}
