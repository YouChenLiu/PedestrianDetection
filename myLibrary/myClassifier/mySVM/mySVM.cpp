#include "mySVM.h"

// initialize the static value
// the value for soft margin
const double mySVM::SVM_CONSTRAINT_VALUE = 1.0;

// the system is converge when diffrence less than it
const double mySVM::EPSILON = 1e-6;

// opencv criteria object
const cv::TermCriteria mySVM::CRITERIA = cv::TermCriteria(
    mySVM::CRITERIA_TYPE,
    mySVM::ITERATION_COUNT,
    mySVM::EPSILON
);

mySVM::mySVM() {}

mySVM::~mySVM() {}

void mySVM::Train(void) {
    // create SVM for learning model
    m_poClassifier = cv::ml::SVM::create();

    // cast ptr to svm
    auto pSVM = m_poClassifier.dynamicCast<cv::ml::SVM>();

    // setting the SVM attribute
    pSVM->setType(mySVM::SVM_Type);
    pSVM->setKernel(mySVM::SVM_KERNEL_Type);
    pSVM->setC(mySVM::SVM_CONSTRAINT_VALUE);
    pSVM->setTermCriteria(mySVM::CRITERIA);

    MakeTrainingData();

    // start training classifier
    pSVM->train(m_poTrainingData);
}

float mySVM::Predict(const std::vector<float>& vfSample) const {
    // get the feature length of sample
    auto iFeatureLength = static_cast<int>(vfSample.size());

    // create matrix for saving feature
    cv::Mat mSample = cv::Mat::zeros(cv::Size2i(iFeatureLength, 1), CV_32FC1);

    // copy feature data to matrix
    for (int x = 0; x < iFeatureLength; x++) {
        mSample.at<float>(0, x) = vfSample.at(x);
    }

    // return the result by calling another method
    return Predict(mSample);
}

void mySVM::Save(const std::string & sDstPath) const {
    if (!m_poClassifier.empty()) {
        m_poClassifier->save(sDstPath);
    }
}

void mySVM::Init(void) {}
