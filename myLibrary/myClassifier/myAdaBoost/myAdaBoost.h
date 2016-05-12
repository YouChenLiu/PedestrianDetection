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

    myAdaBoost(const std::string& sFilePath) {
      Load(sFilePath);
    }

    virtual ~myAdaBoost(void);

    /**
     * @brief Create adaboost from xml file
     *
     * @param sFilePath The XML file path for reading.
     */
    void Load(const std::string& sFilePath) override {
      m_poClassifier = cv::ml::StatModel::load<cv::ml::Boost>(sFilePath);
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

};

#endif // !_MY_ADABOOST_H_