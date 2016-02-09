#ifndef _MY_BLOCK_BASED_EXTRACTOR_H_
#define _MY_BLOCK_BASED_EXTRACTOR_H_

#include "../../common.h"
#include "../myExtractorBase.h"

class myBlockBasedExtractor : public myExtractorBase {
public:     // public attribute

protected:  // protected attribute
    cv::Size2i m_BlockSize;

private:    // private attribute

public:     // public method
    myBlockBasedExtractor(void);
    myBlockBasedExtractor(const cv::Mat& mImage,
                          const cv::Size2i BlockSize = cv::Size2i(8, 8));
    virtual ~myBlockBasedExtractor(void);

    // set the block size for extractor
    void SetBlockSize(const cv::Size2i BlockSize) { m_BlockSize = BlockSize; }

protected:  // protected method

private:    // private method

};

#endif // !_MY_BLOCK_BASED_EXTRACTOR_H_