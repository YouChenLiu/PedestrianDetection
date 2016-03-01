/**
 * @file myHOG.h
 * @brief Histogram of Gradient class definition.
 */

#ifndef _MY_HOG_H_
#define _MY_HOG_H_

#include "../myBlockBasedExtractor/myBlockBasedExtractor.h"

/**
 * @brief HOG feature extractor.
 */
class myHOG : public myBlockBasedExtractor {
public:
    /**
     * @brief definition of difference HOG pattern.
     */
    class Feature : protected myFeatureBase {
    public:
        /// Stadard HOG feature.
        static const int HOG_WITHOUT_NORM = 10;

        /// HOG feature with L1 norm.
        static const int HOG_WITH_L1_NORM = 11;

        /// HOG feature with L1 sqrt.
        static const int HOG_WITH_L1_SQRT = 12;

        /// HOG feature with L2 norm.
        static const int HOG_WITH_L2_NORM = 13;

        /// HOG feature with L2 sqrt.
        static const int HOG_WITH_L2_SQRT = 14;
    };

    /// The value uses to prevent the division of zero exception.
    static const float m_fUnimportantValue;

    /// This value uses to enlarge the feature value after nomalization
    static const int m_iMagnifyingFactor = 100;

private:
    cv::Mat m_mHorizontalGradientImage;
    cv::Mat m_mVerticalGradientImage;
    int m_iInterval;
    int m_iType;

public:
    /**
     * @brief Constructor.
     *
     * @param mImage A image for computing feature.
     * @param iType The HOG pattern.
     * @param BlockSize The region size for extractor feature.
     * @param iInterval The angle of HOG bin.
     */
    myHOG(const cv::Mat& mImage,
          int iType,
          cv::Size2i BlockSize = cv::Size2i(8, 8),
          int iInterval = 20);
    virtual ~myHOG(void);

    void Describe(cv::Point2i Position, std::vector<float>& vfHogFeature) const override;

    void SetImage(const cv::Mat& mImage);

private:
    void Init(void);
    void DescribeCell(const cv::Point2i Position, std::vector<float>& vfHogFeature) const;
    void Normalize(std::vector<float>& vfFeature) const;
};

#endif