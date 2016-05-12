/**
* @file DIPKernel.h
* @brief DIP Kernel class definition.
*/

#ifndef _DIP_KERNEL_H_
#define _DIP_KERNEL_H_

#include "../common.h"

/**
 * @brief Class for processing mask correlation.
 */
class DIPKernel {
public:
    /**
     * @brief Mask types definition.
     */
    enum class Types {
        SIMPLE_X = 0,   //!< HOG mask in x direction
        SIMPLE_Y,       //!< HOG mask in y direction
        SOBEL_X,        //!< Sobel in x direction
        SOBEL_Y,        //!< Sobel in y direction
        CUSTOM          //!< User defined mask
    };

    /// Number of kernels.
    const static int NUMBER_OF_KERNELS = 6;

private:
    Types       m_Type;
    cv::Mat     m_mKernel;
    cv::Point2i m_Anchor;

    /// a set of kernel
    static std::array<cv::Mat, DIPKernel::NUMBER_OF_KERNELS> m_mClassicalKernels;

private:
    void Init(void);
    void SetKernel(void);
    void SetKernel(cv::Mat& mKernel, int row, int col, const float* pData);
    void SetKernel(const cv::Mat& mKernel);
    
public:
    /**
     * @brief Constructor.
     *
     * Constructor with spesify kernel type.
     * @param type Predefined kernel.
     */
    DIPKernel(Types type);

    /**
     * @brief Constructor with a custom kernel type.
     *
     * @param mCustomKernel User defined mask.
     */
    DIPKernel(const cv::Mat& mCustomKernel);
    ~DIPKernel(void);
    
    /**
     * @brief Compute the correlation with DIPKernel.
     *
     * @param mImage A image needs to processing.
     */
    cv::Mat operator*(const cv::Mat& mImage) const;
    
    /**
     * @brief Compute the correlation with DIPKernel.
     *
     * @param mImage A image needs to processing.
     * @param kernel The mask.
     */
    friend cv::Mat operator*(const cv::Mat& mImage, const DIPKernel& kernel) {
        return kernel.operator*(mImage);
    }
    
    /**
     * @brief Compute the correlation with DIPKernel.
     *
     * @param mImage A image needs to processing.
     * @param kernel The mask.
     */
    friend void operator*=(cv::Mat& mImage, const DIPKernel& kernel) {
        mImage = kernel.operator*(mImage);
    }
};

#endif