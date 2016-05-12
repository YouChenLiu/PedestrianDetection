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

  float myAdaBoost::GetWeightedSum(const cv::Mat & mSample) {
    cv::Mat result;
    auto Sum = m_poClassifier->predict(mSample, result, cv::ml::StatModel::RAW_OUTPUT);
    return Sum;
  }

};