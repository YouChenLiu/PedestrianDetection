#ifndef _MY_SVM_H_
#define _MY_SVM_H_

#include "../mySupervisedClassifier/mySupervisedClassifier.h"

class mySVM final : public mySupervisedClassifier {
public:
    mySVM();
    mySVM(const std::string& sFilePath) {
        Load(sFilePath);
    }
    virtual ~mySVM();

    // train a SVM classifier 
    void Train(void) override;

    // predict response for the provided sample encapsulated in opencv matrix
    float Predict(const cv::Mat& mSample) const override {
        return m_poClassifier.empty() ? 0.0f : m_poClassifier->predict(mSample);
    };

    // predict response for the provided sample encapsulated in std::vector
    float Predict(const std::vector<float>& vfSample) const override;

    // save trained SVM to xml file
    void Save(const std::string& sDstPath) const override;

    // load SVM saved in xml file
    void Load(const std::string& sFilePath) override{
        m_poClassifier = cv::ml::StatModel::load<cv::ml::SVM>(sFilePath);
    }

private:
    // use the openCV smart pointer to point SVM object
    cv::Ptr<cv::ml::SVM> m_poClassifier;

    // --------------set SVM attributes-----------------

    // SVM type will determine how to do classification
    // the value can be C_SVC, NU_SVC,  etc..
    static const int SVM_Type = cv::ml::SVM::C_SVC;

    // SVM kernel type will determine the classification method
    // the value can be LINEAR, POLY, RBF, etc..
    static const int SVM_KERNEL_Type = cv::ml::SVM::LINEAR;

    // the value for soft margin
    // initialize in cpp file
    static const double SVM_CONSTRAINT_VALUE;

    // this attribute determine what's situation can stop training
    // COUNT : stop training when reach the max count
    // MAX_ITER : stop training wehn reach max count or converge
    // EPS : stop training when it's converge
    static const int CRITERIA_TYPE = cv::TermCriteria::MAX_ITER;

    // the maximum training cycle count
    static const int ITERATION_COUNT = 10000;

    // the system is converge when diffrence less than it
    // initialize in cpp file
    static const double EPSILON;

    // opencv criteria object
    // initialize in cpp file
    static const cv::TermCriteria CRITERIA;

    // --------------end SVM attributes-----------------

private:
    void Init(void);

};

#endif // !_MY_SVM_H_