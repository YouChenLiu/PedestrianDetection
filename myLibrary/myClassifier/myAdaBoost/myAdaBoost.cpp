#include "myAdaBoost.h"

namespace Classifier {

const double myAdaBoost::TRIM_RATE = 0.95;

myAdaBoost::myAdaBoost(unsigned int iWeakCount) {
  // create adaboost
  auto pBoost = cv::ml::Boost::create();
  m_poClassifier = pBoost;

  // set parameter
  pBoost->setBoostType(myAdaBoost::BOOST_TYPE);
  pBoost->setWeightTrimRate(myAdaBoost::TRIM_RATE);
  pBoost->setWeakCount(iWeakCount);
}

myAdaBoost::~myAdaBoost(void) {}

void myAdaBoost::Load(const std::string & sFilePath) {
    m_poClassifier = cv::ml::StatModel::load<cv::ml::Boost>(sFilePath);
    if (m_poClassifier.empty()) {
      std::cout << "Loading AdaBoost Failed!!" << std::endl;
    }
  }

float myAdaBoost::GetWeightedSum(const cv::Mat& mSample) const {
  auto DTree = GetClassifier().dynamicCast<cv::ml::DTrees>();
  return DTree->predict(mSample, cv::noArray(), cv::ml::DTrees::Flags::PREDICT_SUM);
}

std::vector<int> myAdaBoost::GetIndicate(void) const {
  auto DTree = GetClassifier().dynamicCast<cv::ml::DTrees>();
  if (DTree == nullptr) {
    return std::vector<int>();
  }
  auto splits = DTree->getSplits();
  std::vector<int> viIndicate;
  viIndicate.reserve(splits.size());
  for (const auto& split : splits) {
    viIndicate.push_back(split.varIdx);
  }
  
  return viIndicate;
}
  
}