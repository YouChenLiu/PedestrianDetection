#ifndef _MY_SUPERVISED_CLASSIFIER_H_
#define _MY_SUPERVISED_CLASSIFIER_H_

#include "../myClassifierBase.h"

class mySupervisedClassifier : public myClassifierBase {
public:

public:
    mySupervisedClassifier();
    virtual ~mySupervisedClassifier();

    virtual void AddSample(int iLable, const std::vector<float>& vfFeature);

protected:
    // the array for saving answer labels
    std::vector<int> m_viLabel;

private:

private:

};

#endif // !_MY_SUPERVISED_CLASSIFIER_H_