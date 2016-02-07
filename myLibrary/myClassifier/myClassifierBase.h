#ifndef _MY_CLASSIFIER_BASE_H_
#define _MY_CLASSIFIER_BASE_H_

#include "../common.h"
#include <fstream>
#include <sstream>

// it's a base class for cklassifier
// it contain a vector of vector of float for saving features and
//  some methods for accessing it such as load and save feature
// it also contain some methods for calssifier like train and predict
// if you want to design your classifer, you should inherit two derived class
//  superviesed and unsupervised class
// do not modify this class or modify it carefully, please
class myClassifierBase {
public:     // public attribute

protected:  // protected attribute
    // the array for saving features
    std::vector<std::vector<float>> m_vvfFeature;

private:    // private attribute
    // stream for caching write out data
    std::stringstream FileBuffer;

public:     // public method
    myClassifierBase(void) {};
    virtual ~myClassifierBase(void) {};

    // training the classifier by contaning samples
    virtual void Train(void) = 0;

    // use trained classifier before to classify the sample
    virtual float Predict(const cv::Mat& mSample) const = 0;
    virtual float Predict(const std::vector<float>& vfSample) const = 0;

    // save the trained classifier to an xml file
    virtual void Save(const std::string& sDstPath) const = 0;

    // save features in table to csv file
    virtual void SaveFeatures(const std::string& sDstPath);

    // create classifier from model xml file
    virtual void Load(const std::string& sModelFile) = 0;

    // create classifier from feature xml file
    virtual void LoadFeatures(const std::string& sFeatureFile);

protected:  // protected method
    // add a sample to array
    void AddFeature(const std::vector<float>& vfNewFeature) {
        m_vvfFeature.push_back(vfNewFeature);
    }

    // add a line features saved in string which uses common for seperating
    void AddFeature(const std::string& sLineOfFeature);

    // write data to cache
    void WriteData(const std::string& sData) {
        FileBuffer << sData;
    }

    // tranform feature vector to string
    void WriteFeatureToString(const std::vector<float>& vfFeature,
                              std::string& s) const;

    // write out the caching data to text file
    void WriteOutFile(const std::string& sDstPath);

private:    // private method

};

#endif // !_MY_CLASSIFIER_BASE_H_