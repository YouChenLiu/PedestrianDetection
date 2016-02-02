#ifndef _MY_SUPERVISED_CLASSIFIER_H_
#define _MY_SUPERVISED_CLASSIFIER_H_

#include "../myClassifierBase.h"
#include <opencv2/ml.hpp>

class mySupervisedClassifier : public myClassifierBase {
public:

public:
    mySupervisedClassifier();
    virtual ~mySupervisedClassifier();

    // add a sample and its label to feature table
    virtual void AddSample(int iLable, const std::vector<float>& vfFeature);

    // load feature file from xml file
    void LoadFeature(const std::string& sFeatureFile) override;

protected:
    // the array for saving answer labels
    std::vector<int> m_viLabel;

private:

private:

};

#endif // !_MY_SUPERVISED_CLASSIFIER_H_