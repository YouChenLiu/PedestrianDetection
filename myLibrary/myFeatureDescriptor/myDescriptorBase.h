/**
 * @file myExtractorBase.h
 * @brief Extractor base class definition.
 */

#ifndef _MY_DESCRIPTOR_BASE_H_
#define _MY_DESCRIPTOR_BASE_H_

#include "../common.h"
#include "FeatureDefinition.h"

namespace Descriptor {

  /**
   * @brief Base class for computing the feature.
   */
  class myDescriptorBase {
  public:     // public attribute
    /// define the different pattern for the derived extractor
    class myFeatureBase {
    public:
      /// ERROR number.
      static const int NONE = 0;
    };

  protected:  // protected attribute
    /// The image for extracting feature.
    cv::Mat m_mImage;

    int m_iFeatureType;

    static const int CONFIGURE_OFFSET = 24;
    static const int NORM_OFFSET = 16;
    static const int FEATURE_OFFSET = 8;
    static const int PATTERN_OFFSET = 0;

    static const int CONFIGURE_MASK = 0x7f << CONFIGURE_OFFSET;
    static const int NORM_MASK = 0xff << NORM_OFFSET;
    static const int FEATURE_MASK = 0xff << FEATURE_OFFSET;
    static const int PATTERN_MASK = 0Xff << PATTERN_OFFSET;

    static const int WELL_KNOWN_FEATURE_FLAG = 0x40 << CONFIGURE_OFFSET;
    static const int NORMALIZATION_FLAG = 0x20 << CONFIGURE_OFFSET;
    static const int BLOCK_BASED_FLAG = 0x10 << CONFIGURE_OFFSET;

  private:    // private attribute

  public:     // public method
    /**
     * @brief Default Constructor.
     */
    myDescriptorBase(void) { m_mImage = cv::Mat(); }

    /**
     * @brief Create with a image.
     *
     * @param mImage A image want to compute feature.
     */
    myDescriptorBase(const cv::Mat& mImage) { SetImage(mImage); }
    virtual ~myDescriptorBase(void) {}

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

    const int GetFeatureType(void) const { return m_iFeatureType; }

  protected:  // protectd method

  private:    // private method

  };
};
#endif