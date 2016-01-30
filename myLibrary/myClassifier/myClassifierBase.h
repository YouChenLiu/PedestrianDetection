#ifndef _MY_CLASSIFIER_BASE_H_
#define _MY_CLASSIFIER_BASE_H_

#include "../common.h"
#include <opencv2/ml.hpp>

class myClassifierBase {
public:
    myClassifierBase(void) {};
    ~myClassifierBase(void) {};

    virtual void Train(void) = 0;
    virtual float Predict(const cv::Mat& mSample) const = 0;

protected:
    // the array for saving features
    std::vector<std::vector<float>> m_vvfFeature;

};

#endif // !_MY_CLASSIFIER_BASE_H_
