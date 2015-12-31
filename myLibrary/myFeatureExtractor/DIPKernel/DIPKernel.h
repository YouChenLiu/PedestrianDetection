#ifndef _DIP_KERNEL_H_
#define _DIP_KERNEL_H_

#include <iostream>
#include <array>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class DIPKernel {
public:
    enum class Types {
        SIMPLE_X = 0,       // hog mask in x direction
        SIMPLE_Y,           // hog mask in y direction
        SOBEL_X,            // sobel in x direction
        SOBEL_Y,            // sobel in y direction
        CUSTOM
    };
    const static int NUMBER_OF_KERNELS = 6;

private:
    Types       m_Type;
    cv::Mat     m_mKernel;
    cv::Point2i m_Anchor;
    static std::array<cv::Mat, DIPKernel::NUMBER_OF_KERNELS> m_mClassicalKernels;     // a set of kernel

private:
    void Init(void);
    void SetKernel(void);
    void SetKernel(cv::Mat& mKernel, int row, int col, const float* pData);
    void SetKernel(const cv::Mat& mKernel);
    
public:
    DIPKernel(Types type);                          // constructor with kernel type
    DIPKernel(const cv::Mat& mCustomKernel);        // constructor with a custom kernel type
    ~DIPKernel(void);
    
    cv::Mat operator*(const cv::Mat& mImage) const;
    friend cv::Mat operator*(const cv::Mat& mImage, const DIPKernel& kernel) { return kernel.operator*(mImage); }
    friend void operator*=(cv::Mat& mImage, const DIPKernel& kernel) { mImage = kernel.operator*(mImage); }
};

#endif