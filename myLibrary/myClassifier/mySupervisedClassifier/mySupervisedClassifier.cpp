#include "mySupervisedClassifier.h"

mySupervisedClassifier::mySupervisedClassifier() {}

mySupervisedClassifier::~mySupervisedClassifier() {}

void mySupervisedClassifier::AddSample(int iLable, const std::vector<float>& vfFeature) {
    AddFeature(vfFeature);
    m_viLabel.push_back(iLable);
}

void mySupervisedClassifier::LoadFeature(const std::string & sFeatureFile) {}
