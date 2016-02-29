#ifndef _MY_BLOCK_BASED_EXTRACTOR_H_
#define _MY_BLOCK_BASED_EXTRACTOR_H_

#include "../../common.h"
#include "../myExtractorBase.h"

/**
 * @brief Base class for feature extractor
 * which computes feature in a region.
 */
class myBlockBasedExtractor : public myExtractorBase {
public:     // public attribute

protected:  // protected attribute
    /// The region size for extractor feature.
    cv::Size2i m_BlockSize;

private:    // private attribute

public:     // public method
    /**
     * @brief Default Constructor.
     */
    myBlockBasedExtractor(void);

    /**
     * @brief Create with a image.
     *
     * @param mImage A image for computing feature.
     * @param BlockSize A region size for computing feature.
     */
    myBlockBasedExtractor(const cv::Mat& mImage,
                          const cv::Size2i BlockSize = cv::Size2i(8, 8));
    virtual ~myBlockBasedExtractor(void);

    /**
     * @brief Set the block size for extractor
     */
    void SetBlockSize(const cv::Size2i BlockSize) { m_BlockSize = BlockSize; }

protected:  // protected method

private:    // private method

};

#endif // !_MY_BLOCK_BASED_EXTRACTOR_H_