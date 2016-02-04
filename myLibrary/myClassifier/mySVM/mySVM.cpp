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

mySVM::mySVM() {
    // create SVM
    auto pSVM = cv::ml::SVM::create();
    m_poClassifier = pSVM;

    // setting the SVM attribute
    pSVM->setType(mySVM::SVM_Type);
    pSVM->setKernel(mySVM::SVM_KERNEL_Type);
    pSVM->setC(mySVM::SVM_CONSTRAINT_VALUE);
    pSVM->setTermCriteria(mySVM::CRITERIA);
}

mySVM::~mySVM() {}

void mySVM::Train(void) {
    MakeTrainingData();
    
    // start training classifier
    m_poClassifier.dynamicCast<cv::ml::SVM>()->train(m_poTrainingData);
}