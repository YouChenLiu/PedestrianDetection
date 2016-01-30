#include "mySVM.h"

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

    // number of samples
    auto iNumOfSamples = static_cast<int>(m_vvfFeature.size());

    // the feature length
    auto iFeatureLength = static_cast<int>(m_vvfFeature.at(0).size());

    // matrix for saving features
    cv::Mat mSample = cv::Mat::zeros(
        cv::Size2i(iFeatureLength, iNumOfSamples), CV_32FC1);

    // representation the samples with opencv Mat
    for (int y = 0; y < iNumOfLabels; y++) {
        for (int x = 0; x < iFeatureLength; x++) {
            mSample.at<float>(y, x) = m_vvfFeature.at(y).at(x);
        }
    }

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
