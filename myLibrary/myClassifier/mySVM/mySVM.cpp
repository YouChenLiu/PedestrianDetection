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
    // number of labels
    auto iNumOfLabels = static_cast<int>(m_viLabel.size());

    // matrix for saving labels
    cv::Mat mLabel = cv::Mat::zeros(cv::Size2i(1, iNumOfLabels), CV_32SC1);

    // representation the labels with opencv Mat
    for (int y = 0; y < static_cast<int>(m_viLabel.size()); ++y) {
        mLabel.at<int>(y, 0) = m_viLabel.at(y);
    }

    // clear labels in vector for reducing memory
    m_viLabel.clear();

    // number of samples
    auto iNumOfSamples = static_cast<int>(m_vvfFeature.size());

    // the feature length
    auto iFeatureLength = static_cast<int>(m_vvfFeature.at(0).size());

    // matrix for saving features
    cv::Mat mSample = cv::Mat::zeros(cv::Size2i(iFeatureLength, iNumOfSamples),
                                     CV_32FC1);

    // representation the samples with opencv Mat
    for (int y = 0; y < iNumOfLabels; y++) {
        for (int x = 0; x < iFeatureLength; x++) {
            // copy the features from vector to opencv matrix
            mSample.at<float>(y, x) = m_vvfFeature.at(y).at(x);
        }

        // clear the copyed feature for reducing memory
        m_vvfFeature.at(y).clear();
    }

    // clear feature table for reducing memory
    m_vvfFeature.clear();

    // create SVM for learning model
    m_poClassifier = cv::ml::SVM::create();

    // setting the SVM attribute
    m_poClassifier->setType(mySVM::SVM_Type);
    m_poClassifier->setKernel(mySVM::SVM_KERNEL_Type);
    m_poClassifier->setC(mySVM::SVM_CONSTRAINT_VALUE);
    m_poClassifier->setTermCriteria(mySVM::CRITERIA);

    // start training classifier
    m_poClassifier->train(mSample, cv::ml::ROW_SAMPLE, mLabel);
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
