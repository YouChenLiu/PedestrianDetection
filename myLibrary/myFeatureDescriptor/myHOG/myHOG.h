/**
 * @file myHOG.h
 * @brief Histogram of Gradient class definition.
 */

#ifndef _MY_HOG_H_
#define _MY_HOG_H_

#include "../myBlockDescriptorBase.h"

namespace Descriptor {

  /**
   * @brief HOG feature extractor.
   */
  class myHOG : public myBlockDescriptorBase {
  protected:
    static const int HOG = WELL_KNOWN_FEATURE_FLAG | BLOCK_BASED_FLAG |
      (HOG_FEATURE << FEATURE_OFFSET);

  public:
    /**
     * @brief definition of difference HOG pattern.
     */
    class Feature : protected myFeatureBase {
    public:
      /// Stadard HOG feature.
      static const int HOG_STANDARD = HOG;
      static const int HOG_SINGLE_CELL = HOG_STANDARD | (0x01 << PATTERN_OFFSET);
    };

  private:
    cv::Mat m_mHorizontalGradientImage;
    cv::Mat m_mVerticalGradientImage;
    int m_iInterval;

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
    void DescribeCell(const cv::Point2i Position,
                      cv::Size2i CellSize,
                      std::vector<float>& vfHogFeature) const;
  };

};
#endif