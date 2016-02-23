#ifndef _MY_LBP_INDEXER_H_
#define _MY_LBP_INDEXER_H_

#include "../myModelIndexerBase.h"
#include "../../myFeatureExtractor/myLBP/myLBP.h"

class myLBPIndexer final : public myModelIndexerBase, protected myLBP {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
    static std::array<unsigned int, 256> m_aiUniformIndex;

public:     // public method
    myLBPIndexer(cv::Size2i BlockSize = cv::Size2i(8, 8));
    virtual ~myLBPIndexer(void);

    // return the maximun number of bins
    unsigned int GetNumOfBins(void) const override { return 59u; }

    // calculate the bin number with specific position
    unsigned int GetBinNumber(const cv::Mat& mImage,
                              const cv::Point2i Position) const override;

protected:  // protected method

private:    // private method
    

    void Init(void);
};

#endif // !_MY_LBP_INDEXER_H_