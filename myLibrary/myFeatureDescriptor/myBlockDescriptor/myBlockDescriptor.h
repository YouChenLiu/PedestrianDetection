/**
 * @file myBlockDescriptor.h
 * @brief block descriptor class definition.
 */

#ifndef _MY_BLOCK_DESCRIPTOR_H_
#define _MY_BLOCK_DESCRIPTOR_H_

#include <memory>
#include "../myBlockDescriptorBase.h"
#include "../myHOG/myHOG.h"
#include "../myLBP/myLBP.h"

namespace Descriptor {

  /**
   * @brief Class for computing any block-based feature.
   */
  class myBlockDescriptor : public myBlockDescriptorBase {
  public:
    /// The predefine pattern value.
    class Feature : public myHOG::Feature, public myLBP::Feature {
    public:
      /// L1 norm flag
      static const int L1_NORM = NORMALIZATION_FLAG | (0x01 << NORM_OFFSET);
      /// L1 sqrt flag
      static const int L1_SQRT = NORMALIZATION_FLAG | (0x02 << NORM_OFFSET);
      /// L2 norm flag
      static const int L2_NORM = NORMALIZATION_FLAG | (0x03 << NORM_OFFSET);
      /// L2 sqrt flag
      static const int L2_SQRT = NORMALIZATION_FLAG | (0x04 << NORM_OFFSET);
    };

  private:
    /// The vector of extractors.
    std::vector<std::unique_ptr<Descriptor::myDescriptorBase>> m_vpoUsedDescriptor;

    /// This value uses to enlarge the feature value after nomalization
    static const int m_iMagnifyingFactor = 100;
    
    /// The value uses to prevent the division of zero exception.
    static const float m_fUnimportantValue;

  public:
    /**
     * @brief Create with a image.
     *
     * @param mImage A image for computing feature.
     * @param BlockSize A region size for computing feature.
     */
    myBlockDescriptor(const cv::Mat& mImage, cv::Size2i BlockSize = cv::Size2i(8, 8));

    /**
     * @brief Create with a IplImage.
     *
     * @param pImage A image for computing feature.
     * @param BlockSize A region size for computing feature.
     */
    myBlockDescriptor(IplImage* pImage, CvSize BlockSize = cvSize(8, 8));
    ~myBlockDescriptor(void);

    void Describe(cv::Point2i Position, std::vector<float>& vfFeature) const;

    /**
     * @brief Add new extractor for extracting feature.
     *
     * @param iFeature The predefine pattern in Features
     */
    void EnableFeature(int iFeature);

    /**
     * @brief Set the block size.
     *
     * @param Size The new size.
     */
    void SetBlockSize(cv::Size2i Size) { m_BlockSize = Size; }

    /**
     * @brief Set the block size.
     *
     * @param Width The width in new blick size.
     * @param Height The height in new block size.
     */
    void SetBlockSize(int Width, int Height) { SetBlockSize(cv::Size2i(Width, Height)); }

    void SetImage(const cv::Mat& mImg) override;

  private:
    void Init(void);
    void Normalize(int iNormMethod, std::vector<float>& vfFeature) const;
  };

};
#endif