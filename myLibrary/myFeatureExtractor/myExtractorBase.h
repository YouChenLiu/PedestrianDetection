#ifndef _MY_EXTRACTOR_BASE_H_
#define _MY_EXTRACTOR_BASE_H_

#include "../common.h"

/**
 * @brief Base class for computing the feature.
 */
class myExtractorBase {
public:     // public attribute
    /// define the different pattern for the derived extractor
    class myFeatureBase {
    public:
        /// ERROR number.
        static const int NONE = -1;
    };

protected:  // protected attribute
    /// The image for extracting feature.
    cv::Mat m_mImage;

private:    // private attribute

public:     // public method
    /**
     * @brief Default Constructor.
     */
    myExtractorBase(void);

    /**
     * @brief Create with a image.
     *
     * @param mImage A image want to compute feature.
     */
    myExtractorBase(const cv::Mat& mImage);
    virtual ~myExtractorBase(void);

    /**
     * @brief Describe the feature at sepecify position.
     *
     * @param position A left-top point for description feature.
     * @param vfFeature The vector will store feature.
     */
    virtual void Describe(cv::Point2i position,
                          std::vector<float>& vfFeature) const = 0;

    /**
     * @brief Describe the feature at sepecify position.
     *
     * @param x A X value of left-top point for description feature.
     * @param y A Y value of left-top point for description feature.
     * @param vfFeature The vector will store feature.
     */
    void Describe(int x, int y, std::vector<float>& vfFeature) const {
        Describe(cv::Point2i(x, y), vfFeature);
    }

    /**
     * @brief Setting the image want to extract feature.
     *
     * @param mImage New image.
     */
    virtual void SetImage(const cv::Mat& mImage) { m_mImage = mImage; }

protected:  // protectd method

private:    // private method

};

#endif