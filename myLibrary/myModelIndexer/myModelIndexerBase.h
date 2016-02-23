#ifndef _MY_MODEL_INDEXER_BASE_H_
#define _MY_MODEL_INDEXER_BASE_H_

#include "../common.h"

class myModelIndexerBase {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute

public:     // public methid
    myModelIndexerBase(void) {}
    virtual ~myModelIndexerBase(void) {}

    virtual unsigned int GetNumOfBins(void) const = 0;

    virtual unsigned int GetBinNumber(const cv::Mat& mImage, 
                                      const cv::Point2i Position) const = 0;

protected:  // protected method

private:    // private method

};

#endif // !_MY_MODEL_INDEXER_BASE_H_