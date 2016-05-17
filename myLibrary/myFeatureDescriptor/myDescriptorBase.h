/**
 * @file myDescriptorBase.h
 * @brief Descriptor base class definition.
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

  /**
   * @brief feature number
   *
   * The value is defined by following rules.
   * each byte of integer has different meaning like the figure shows.
   * <pre>
   * -----------------------------------------------------
   * | Configure |    Norm    |   Feature   |   Pattern  |
   * -----------------------------------------------------
   * 31          24           16            8            0
   * </pre>
   * Configure:
   *   this byte presents what characteristic of this feature.\n
   *   The 31th bit is reversed.
   *   30th to 28th bits are flags include well-known, normalization
   *    block based. The 4 bits left are not used now.
   *
   * Norm:
   *   This byte presents the feature needs normalization after descriptor
   *   or not.
   *
   * Feature:
   *   The feature number. The number is defined in other file named
   *   FeatureDefinition.h.
   *
   * Pattern:
   *   The different number means different pattern for computing simlar feature.
   */
  int m_iFeatureType;

  /// pre-define shift configure offset
  static const int CONFIGURE_OFFSET = 24;
  /// pre-define shift normalization
  static const int NORM_OFFSET = 16;
  /// pre-define shift feature
  static const int FEATURE_OFFSET = 8;
  /// pre-define shift pattern
  static const int PATTERN_OFFSET = 0;

  /// mask for configure
  static const int CONFIGURE_MASK = 0x7f << CONFIGURE_OFFSET;
  /// mask for norm
  static const int NORM_MASK = 0xff << NORM_OFFSET;
  /// mask for feature
  static const int FEATURE_MASK = 0xff << FEATURE_OFFSET;
  /// mask for pattern
  static const int PATTERN_MASK = 0Xff << PATTERN_OFFSET;

  /// a flag is set if this feature is a popular feature
  static const int WELL_KNOWN_FEATURE_FLAG = 0x40 << CONFIGURE_OFFSET;
  /// a flag is set if the feature needs normalization after computing.
  static const int NORMALIZATION_FLAG = 0x20 << CONFIGURE_OFFSET;
  /// a flag is set if this feature is computing in a block.
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

  /**
   * @brief Get the feature number.
   *
   * @return The feature saved in m_iFeatureType.
   */
  const int GetFeatureType(void) const { return m_iFeatureType; }

  /**
   * @brief Check the feature need normalize or not.
   */
  const bool IsNorm(void) const {
    return (m_iFeatureType & NORMALIZATION_FLAG) ? true : false;
  }

  /**
   * @brief Get the norm type of feature.
   */
  const int GetNormType(void) {
    return m_iFeatureType & NORM_MASK | NORMALIZATION_FLAG;
  }

protected:  // protectd method

private:    // private method

};  // class

} // namespace
#endif