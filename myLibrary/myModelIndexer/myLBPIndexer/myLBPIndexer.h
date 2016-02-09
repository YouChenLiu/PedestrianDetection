#ifndef _MY_LBP_INDEXER_H_
#define _MY_LBP_INDEXER_H_

#include "../myModelIndexerBase.h"
#include "../../myFeatureExtractor/myLBP/myLBP.h"

class myLBPIndexer final : public myModelIndexerBase, public myLBP {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute

public:     // public method
    myLBPIndexer(void);
    myLBPIndexer(const cv::Mat& mImage);
    virtual ~myLBPIndexer(void);

    unsigned int GetNumOfBins(void) const override { return 59u; }

    unsigned int GetBinNumber(const cv::Point2i Position) const override;

    
protected:  // protected method

private:    // private method
    void Describe(cv::Point2i Position,
                  std::vector<float>& vfFeature) const override {}

    void Init(void);
};

#endif // !_MY_LBP_INDEXER_H_