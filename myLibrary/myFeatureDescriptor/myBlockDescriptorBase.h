/**
 * @file myBlockDescriptorBase.h
 * @brief Block based descriptor base class definition.
 */

#ifndef _MY_BLOCK_DESCRIPTOR_BASE_H_
#define _MY_BLOCK_DESCRIPTOR_BASE_H_

#include "common.h"
#include "myDescriptorBase.h"

namespace Descriptor {

  /**
   * @brief Base class for feature descriptor
   * which computes feature in a region.
   */
  class myBlockDescriptorBase : public myDescriptorBase {
  public:     // public attribute

  protected:  // protected attribute
    /// The region size for extractor feature.
    cv::Size2i m_BlockSize;

  private:    // private attribute

  public:     // public method
    /**
     * @brief Default Constructor.
     */
    myBlockDescriptorBase(void) : myDescriptorBase() {
      SetBlockSize(cv::Size2i(8, 8));
    }

    /**
     * @brief Create with a image.
     *
     * @param mImage A image for computing feature.
     * @param BlockSize A region size for computing feature.
     */
    myBlockDescriptorBase(const cv::Mat& mImage,
                          const cv::Size2i BlockSize = cv::Size2i(8, 8)) :
      myDescriptorBase(mImage) {
      m_BlockSize = BlockSize;
    }
    virtual ~myBlockDescriptorBase(void) {}

    /**
     * @brief Set the block size for extractor
     *
     * @param BlockSize The new block size.
     */
    void SetBlockSize(const cv::Size2i BlockSize) { m_BlockSize = BlockSize; }

  protected:  // protected method

  private:    // private method

  };

};
#endif // !_MY_BLOCK_BASED_EXTRACTOR_H_
