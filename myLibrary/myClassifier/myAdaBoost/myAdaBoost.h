/**
 * @file myAdaboost.h
 * @brief AdaBoost class definition
 */

#ifndef _MY_ADABOOST_H_
#define _MY_ADABOOST_H_

#include "../mySupervisedClassifier/mySupervisedClassifier.h"

namespace Classifier {

/**
 * @brief AdaBoost classifier
 *
 * myAdaBoost is a warped class.
 * Let's use opencv boosting more convenience.
 */
class myAdaBoost final : public mySupervisedClassifier {
public:
  /**
   * @brief Create classifier with specify weak classifier count.
   *
   * @param iWeakCount The weak classifiers count
   */
  myAdaBoost(unsigned int iWeakCount);

  /**
   * @brief Create classifier by saved xml file.
   *
   * @param sFilePath File path for xml.
   */
  myAdaBoost(const std::string& sFilePath) {
    Load(sFilePath);
  }

  virtual ~myAdaBoost(void);

  /**
   * @brief Create adaboost from xml file
   *
   * @param sFilePath The XML file path for reading.
   */
  void Load(const std::string& sFilePath) override;

  /**
   * @brief Get the AdaBoost raw value
   *
   * @param mSample A Sample saved in cv::Mat which want to predict
   */
  float GetWeightedSum(const cv::Mat& mSample) const;

  /**
   * @brief Get the AdaBoost raw value
   *
   * @param vfSample A Sample saved in std::vector which want to predict
   */
  float GetWeightedSum(const std::vector<float>& vfSample) const {
    return GetWeightedSum(ConvertVecToMat(vfSample));
  }

  /**
   * @brief Get indicate of the weak classifiers
   */
  std::vector<int> GetIndicate(void) const;

  /**
   * @brief Set the number of weak classifiers
   *
   * @param iWeakCount weak classifiers count
   */
  void SetWeakCount(int iWeakCount) {
    m_poClassifier.dynamicCast<cv::ml::Boost>()->setWeakCount(iWeakCount);
  }

private:
  /**
   * @brief AdaBoost types.
   *
   * It can be the one of following values
   * DISCRETE, REAL(default), LOGIT or GENTLE.
   */
  static const int BOOST_TYPE = cv::ml::Boost::Types::REAL;

  /**
   * @brief A threshold for trim.
   *
   * A value between 0 and 1 used to reduce computational time.
   * Training will stop when samples with summary weight <= 1 - TRIM_RATE.
   * Default value is 0.95, 0 for turning off.
   */
  static const double TRIM_RATE;

private:

};

}

#endif // !_MY_ADABOOST_H_