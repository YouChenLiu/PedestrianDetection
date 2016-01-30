#include "mySupervisedClassifier.h"

mySupervisedClassifier::mySupervisedClassifier() {}

mySupervisedClassifier::~mySupervisedClassifier() {}

void mySupervisedClassifier::AddSample(int iLable, const std::vector<float>& vfFeature) {
    m_vvfFeature.push_back(vfFeature);
    m_viLabel.push_back(iLable);
}
