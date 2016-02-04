#ifndef _MY_CLASSIFIER_BASE_H_
#define _MY_CLASSIFIER_BASE_H_

#include "../common.h"
#include <fstream>
#include <sstream>

class myClassifierBase {
public:
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
    virtual void SaveFeature(const std::string& sDstPath) const;

    // create classifier from model xml file
    virtual void Load(const std::string& sModelFile) = 0;

    // create classifier from feature xml file
    virtual void LoadFeature(const std::string& sFeatureFile);

protected:
    // the array for saving features
    std::vector<std::vector<float>> m_vvfFeature;

protected:
    // add a sample to array
    void AddFeature(const std::vector<float>& vfNewFeature) {
        m_vvfFeature.push_back(vfNewFeature);
    }

    // add a line features saved in string which uses common for seperating
    void AddFeature(const std::string& sLineOfFeature);

    // write feature string to DstFile
    void SaveFeature(std::ofstream& DstFile,
                     const std::string& sLineOfFeature) const {
        DstFile << sLineOfFeature << std::endl;
    }

    // tranform feature vector to string
    void WriteFeatureToString(const std::vector<float>& vfFeature,
                              std::string& s) const;

};

#endif // !_MY_CLASSIFIER_BASE_H_
