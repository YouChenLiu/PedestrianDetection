#ifndef _MY_MODEL_INDEXER_BASE_H_
#define _MY_MODEL_INDEXER_BASE_H_

#include "../common.h"

/**
 * @brief Base class for calculating bin number.
 */
class myModelIndexerBase {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute

public:     // public methid
    /**
     * @brief Deafult constructor.
     */
    myModelIndexerBase(void) {}
    virtual ~myModelIndexerBase(void) {}

    /**
    * @brief Get the maximun number of bins.
    */
    virtual unsigned int GetNumOfBins(void) const = 0;

    /**
    * @brief Calculate the bin number with specific position.
    *
    * @param mImage The image for calculating index.
    * @param Position A left-top point for description feature.
    */
    virtual unsigned int GetBinNumber(const cv::Mat& mImage, 
                                      const cv::Point2i Position) const = 0;

protected:  // protected method

private:    // private method

};

#endif // !_MY_MODEL_INDEXER_BASE_H_