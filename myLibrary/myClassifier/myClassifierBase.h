#ifndef _MY_CLASSIFIER_BASE_H_
#define _MY_CLASSIFIER_BASE_H_

#include "../common.h"
#include <opencv2/ml.hpp>

class myClassifierBase {
public:
    myClassifierBase(void) {};
    ~myClassifierBase(void) {};

    // training the classifier by contaning samples
    virtual void Train(void) = 0;

    // use trained classifier before to classify the sample
    virtual float Predict(const cv::Mat& mSample) const = 0;
    virtual float Predict(const std::vector<float>& vfSample) const = 0;

    // save the trained classifier to an xml file
    virtual void Save(const std::string& sDstPath) const = 0;

    // create classifier from xml file
    virtual void Load(const std::string& sFilePath) = 0;

protected:
    // the array for saving features
    std::vector<std::vector<float>> m_vvfFeature;

};

#endif // !_MY_CLASSIFIER_BASE_H_
