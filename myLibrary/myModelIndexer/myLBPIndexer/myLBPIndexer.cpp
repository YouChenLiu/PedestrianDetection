#include "myLBPIndexer.h"

std::array<unsigned int, 256> myLBPIndexer::m_aiUniformIndex;

myLBPIndexer::myLBPIndexer(cv::Size2i BlockSize) :
    myLBP(cv::Mat(), myLBP::Feature::LBP_8_1_UNIFORM, BlockSize) {
    unsigned int iMapping = 0;
    for (unsigned int i = 0; i < 256; ++i) {
        auto bUniform = IsUniform(Feature::LBP_8_1_UNIFORM, i);
        m_aiUniformIndex.at(i) = bUniform ? iMapping++ : 58;
    }
}

myLBPIndexer::~myLBPIndexer(void) {}

unsigned int myLBPIndexer::GetBinNumber(const cv::Mat& mImage,
                                        const cv::Point2i Position) const {
    // the result will be returnd
    unsigned int iBinNumber = 0;
    
    // center block region
    cv::Rect2i CenterRect(Position, m_BlockSize);

    // calculate the average of center region
    auto fCenterAverage = cv::mean(mImage(CenterRect))[0];
    
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
            cv::Point2i ptStart(Position.x + x * m_BlockSize.width,
                                Position.y + y * m_BlockSize.height);
            cv::Rect2i AvgRange(ptStart, m_BlockSize);
            cv::Mat mSubImg = mImage(AvgRange);
            auto fAverage = cv::mean(mSubImg)[0];

            // set corresponding position to 0 if the avg. is less then center
            unsigned char cResult = (fAverage <= fCenterAverage) ? 0 : 255u;
            mImg.at<unsigned char>(y + 1, x + 1) = cResult;
        }
    }

    // use LBP argorithm fet getting Index
    iBinNumber = myLBP::GetBinNumber(mImg);

    return m_aiUniformIndex.at(iBinNumber);
}

void myLBPIndexer::Init(void) {}