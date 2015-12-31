#include "DIPKernel.h"

std::array<cv::Mat, DIPKernel::NUMBER_OF_KERNELS> DIPKernel::m_mClassicalKernels;

DIPKernel::DIPKernel(Types type) {
    Init();
    m_Type = type;
    m_mKernel = m_mClassicalKernels.at(static_cast<int>(type)).clone();
}

DIPKernel::DIPKernel(const cv::Mat& mCustomKernel) {
    Init();
    m_Type = Types::CUSTOM;
    SetKernel(mCustomKernel);
}

DIPKernel::~DIPKernel(void) {}

void DIPKernel::Init(void) {
    if (m_mClassicalKernels.at(0).empty() == true) {
        SetKernel();
    }
    m_mKernel = cv::Mat();
    m_Anchor = cv::Point2i(-1, -1);
}

void DIPKernel::SetKernel(void) {
    // init simple y kernel
    float simpleX[9] = {  0.0f, 0.0f, 0.0f,
                         -1.0f, 0.0f, 1.0f,
                          0.0f, 0.0f, 0.0f };
    SetKernel(m_mClassicalKernels.at(static_cast<int>(Types::SIMPLE_X)), 3, 3, simpleX);

    // init simple y kernel
    float simpleY[9] = { 0.0f, -1.0f, 0.0f,
                         0.0f,  0.0f, 0.0f,
                         0.0f,  1.0f, 0.0f };
    SetKernel(m_mClassicalKernels.at(static_cast<int>(Types::SIMPLE_Y)), 3, 3, simpleY);

    // init Sobel X
    float sobelX[9] = { -1.0f, 0.0f, +1.0f,
                        -2.0f, 0.0f, +2.0f,
                        -1.0f, 0.0f, +1.0f };
    SetKernel(m_mClassicalKernels.at(static_cast<int>(Types::SOBEL_X)), 3, 3, sobelX);

    // init Sobel Y
    float sobelY[9] = { -1.0f, -2.0f, -1.0f,
                        +0.0f, +0.0f, +0.0f,
                        +1.0f, +2.0f, +1.0f };
    SetKernel(m_mClassicalKernels.at(static_cast<int>(Types::SOBEL_Y)), 3, 3, sobelY);
}

inline
void DIPKernel::SetKernel(cv::Mat& mKernel, int row, int col, const float* pData) {
    mKernel = cv::Mat(row, col, CV_32FC1);
    for (int i = 0; i < row * col; i++) {
        mKernel.at<float>(i) = pData[i];
    }
}

inline
void DIPKernel::SetKernel(const cv::Mat& mKernel) {
    m_mKernel = mKernel.clone();
}

cv::Mat DIPKernel::operator*(const cv::Mat& mImage) const {
    cv::Mat mSource;
    switch (mImage.type()) {
    case CV_8UC1:
        mImage.convertTo(mSource, CV_16SC1);
        /*
        if (m_Type == DIPKernelTypes::SIMPLE_X || m_Type == DIPKernelTypes::SIMPLE_Y) {
            mImage.convertTo(mSource, CV_16SC1);
        } else {
            mSource = mImage;
        }
        */
        break;
    default:
        std::cout << "not support this image type" << std::endl;
        return cv::Mat();
        break;
    }
    cv::Mat mResult = cv::Mat(mImage.rows, mImage.cols, mSource.type());
    filter2D(mSource, mResult, mSource.depth(), m_mKernel, m_Anchor);
    return mResult;
}
