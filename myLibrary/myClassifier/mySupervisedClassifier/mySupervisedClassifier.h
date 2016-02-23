#ifndef _MY_SUPERVISED_CLASSIFIER_H_
#define _MY_SUPERVISED_CLASSIFIER_H_

#include "../myClassifierBase.h"
#include <opencv2/ml.hpp>

// it a base class for supervised learing classifier
// it uses machine learning module in opencv reduce the work
// you can inherit it and design your classifier without worring about
//  the features and labels
class mySupervisedClassifier : public myClassifierBase {
public:     // public attribute

protected:  // protected attribute
    // the array for saving answer labels
    std::vector<int> m_viLabel;

    // use openCV smart pointer to point classifier
    cv::Ptr<cv::ml::StatModel> m_poClassifier;

    // use openCV smart pointer to point training data
    cv::Ptr<cv::ml::TrainData> m_poTrainingData;

public:     // public method
    mySupervisedClassifier();
    virtual ~mySupervisedClassifier();

    // add a sample with its label to feature and label array
    virtual void AddSample(int iLable, const std::vector<float>& vfFeature);

    // save labels and features in array to csv file
    virtual void SaveFeatures(const std::string& sDstPath) override;

    // load feature file from xml file
    void LoadFeatures(const std::string& sFeatureFile) override;

    // get the classidier pointer
    const cv::Ptr<cv::ml::StatModel> GetClassifier(void) const {
        return m_poClassifier;
    }

    // train the supervised classifier
    virtual void Train(void) override;

    // predict response for the provided sample encapsulated in opencv matrix
    float Predict(const cv::Mat& mSample) const override {
        return m_poClassifier->empty() ? NAN : m_poClassifier->predict(mSample);
    }

    // predict response for the provided sample encapsulated in std::vector
    float Predict(const std::vector<float>& vfSample) const override;

    // save trained SVM to xml file
    bool Save(const std::string& sDstPath) const override;

protected:  // protected method
    // add a label to array
    void AddLabel(int iLabel) { m_viLabel.push_back(iLabel); }

    // tranform labels and features to opencv matrix
    // and create opencv training data for training classifier
    void MakeTrainingData(void);

private:    // private method
    // add a feature string with its label to label and feature array
    void AddSample(int iLable, const std::string& sLoneOfString);
};

#endif // !_MY_SUPERVISED_CLASSIFIER_H_