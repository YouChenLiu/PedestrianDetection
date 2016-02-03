#ifndef _MY_SUPERVISED_CLASSIFIER_H_
#define _MY_SUPERVISED_CLASSIFIER_H_

#include "../myClassifierBase.h"
#include <opencv2/ml.hpp>

class mySupervisedClassifier : public myClassifierBase {
public:

public:
    mySupervisedClassifier();
    virtual ~mySupervisedClassifier();

    // add a sample with its label to feature and label array
    virtual void AddSample(int iLable, const std::vector<float>& vfFeature);

    // save labels and features in array to csv file
    virtual void SaveFeature(const std::string& sDstPath) const override;

    // load feature file from xml file
    void LoadFeature(const std::string& sFeatureFile) override;

protected:
    // the array for saving answer labels
    std::vector<int> m_viLabel;

protected:
    // add a label to array
    void AddLabel(int iLabel) { m_viLabel.push_back(iLabel); }

    // write label to DstFile
    void SaveLabel(std::ofstream& DstFile, int iLabel) const {
        DstFile << iLabel << ": ";
    }

private:

private:
    // add a feature string with its label to label and feature array
    void AddSample(int iLable, const std::string& sLoneOfString);
};

#endif // !_MY_SUPERVISED_CLASSIFIER_H_