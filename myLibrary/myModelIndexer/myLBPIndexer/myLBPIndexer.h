/**
 * @file myLBPIndexer.h
 * @brief LBP Indexer class definition.
 */

#ifndef _MY_LBP_INDEXER_H_
#define _MY_LBP_INDEXER_H_

#include "../myModelIndexerBase.h"
#include "../../myFeatureDescriptor/myLBP/myLBP.h"

/**
 * @brief Class for calculating index.
 *
 * This class is a indexer for getting bin number by LBP algorithm.
 */
class myLBPIndexer final : public myModelIndexerBase, protected Descriptor::myLBP {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
  /// The index for mapping bin number.
  static std::array<unsigned int, 256> m_aiMappingIndex;

  cv::Size2i m_ImgSize;

public:     // public method
  /**
   * @brief Default Constructor.
   */
  myLBPIndexer();
  virtual ~myLBPIndexer(void);

  unsigned int GetNumOfBins(void) const override { return 59u; }

  unsigned int GetBinNumber(const cv::Mat& mImage,
                            const cv::Rect2i Region) const;

protected:  // protected method

private:    // private method
  void Init(void);
  cv::Rect2i CalRegion(const cv::Point2i ptStart, const cv::Size2i Size) const;
};

#endif // !_MY_LBP_INDEXER_H_