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

float myAdaBoost::GetWeightedSum(const cv::Mat& mSample) const {
  auto DTree = GetClassifier().dynamicCast<cv::ml::DTrees>();
  return DTree->predict(mSample, cv::noArray(), cv::ml::DTrees::Flags::PREDICT_SUM);
}

std::vector<int> myAdaBoost::GetIndicate(void) const {
  auto DTree = GetClassifier().dynamicCast<cv::ml::DTrees>();
  auto splits = DTree->getSplits();
  std::vector<int> viIndicate;
  viIndicate.reserve(splits.size());
  for (const auto& split : splits) {
    viIndicate.push_back(split.varIdx);
  }
  
  return viIndicate;
}
  
}