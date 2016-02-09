#ifndef _MY_SVM_MODEL_H_
#define _MY_SVM_MODEL_H_

#include ""

class mySVMModel : public myStatisticalModelBase {
public:

public:
    mySVMModel(void);
    mySVMModel(std::string sFilePath);
    virtual ~mySVMModel(void);

    void Train(void) override;
    float Predict(const cv::Mat& mSample) const override {
        return m_poClassifier->predict(mSample);
    };
    void SaveTo(const std::string& sDstPath);

private:
    // the array for saving features
    std::vector<std::vector<float>> m_vvfFeature;

    // the array for saving answer labels
    std::vector<int> m_viLabel;

    // the pointer to the SVM
    cv::Ptr<cv::ml::SVM> m_poClassifier;

    // --------------set SVM attributes-----------------

    // SVM type will determine how to do classification
    // the value can be C_SVC, NU_SVC,  etc..
    static const int SVM_Type = cv::ml::SVM::C_SVC;

    // SVM kernel type will determine the classification method
    // the value can be LINEAR, POLY, RBF, etc..
    static const int SVM_KERNEL_Type = cv::ml::SVM::LINEAR;

    // the value for soft margin
    static const double SVM_CONSTRAINT_VALUE;

    // this attribute determine what's situation can stop training
    // COUNT : stop training when reach the max count
    // MAX_ITER : stop training wehn reach max count or converge
    // EPS : stop training when it's converge
    static const int CRITERIA_TYPE = cv::TermCriteria::MAX_ITER;

    // the maximum training cycle count
    static const int ITERATION_COUNT = 10000;

    // the system is converge when diffrence less than it
    static const double EPSILON;

    // opencv criteria object
    static const cv::TermCriteria CRITERIA;

    // --------------end SVM attributes-----------------

private:
    void Init(void);
    void LoadFromFile(std::string sPath);
};

// initialize the static value
const double mySVMModel::SVM_CONSTRAINT_VALUE = 1.0;
const double mySVMModel::EPSILON = 1e-6;
const cv::TermCriteria mySVMModel::CRITERIA = cv::TermCriteria(
    mySVMModel::CRITERIA_TYPE,
    mySVMModel::ITERATION_COUNT,
    mySVMModel::EPSILON);

#endif // !_MY_SVM_MODEL_H_