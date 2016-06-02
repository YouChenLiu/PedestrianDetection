/**
 * @file mySupervisedClassifier.h
 * @brief Supervised Classifier class definition.
 */

#ifndef _MY_SUPERVISED_CLASSIFIER_H_
#define _MY_SUPERVISED_CLASSIFIER_H_

#include "../myClassifierBase.h"
#include <opencv2/ml.hpp>

namespace Classifier {

/**
 * @brief Base class for supervised classifier.
 *
 * It's a base class for supervised learing classifier.
 * It uses machine learning module in opencv reduce the work.
 * You can inherit it and design your classifier without worring about
 * the features and labels.
 */
class mySupervisedClassifier : public myClassifierBase {
public:     // public attribute

protected:  // protected attribute
  /// The array for saving answer labels.
  std::vector<int> m_viLabel;

  /// The openCV smart pointer to point classifier.
  cv::Ptr<cv::ml::StatModel> m_poClassifier;

public:     // public method
  /**
   * @brief Default constructor
   */
  mySupervisedClassifier();
  virtual ~mySupervisedClassifier();

  /**
   * @brief Add a sample with its label to feature and label array.
   *
   * @param iLabel The label describes feature belongs to which class.
   * @param vfFeature The feature wanted to add to table.
   */
  virtual void AddSample(int iLabel, const std::vector<float>& vfFeature);

  virtual void SaveFeatures(const std::string& sDstPath) override;

  void LoadFeatures(const std::string& sFeatureFile) override;

  /**
   * @brief Get the classidier pointer.
   *
   * @return A OpenCV smart pointer to classifier.
   */
  const cv::Ptr<cv::ml::StatModel> GetClassifier(void) const {
    return m_poClassifier;
  }

  virtual void Train(void) override;

  float Predict(const cv::Mat& mSample) const override {
    return m_poClassifier->empty() ? NAN : m_poClassifier->predict(mSample);
  }

  float Predict(const std::vector<float>& vfSample) const {
    return Predict(ConvertVecToMat(vfSample));
  }

  bool Save(const std::string& sDstPath) const override;

  /**
   * @brief Test the classifier is empty or not.
   */
  const bool IsEmpty(void) const {
    return m_poClassifier->empty();
  }

protected:  // protected method
  /**
   * @brief Add a label to array.
   *
   * @param iLabel The label want to add to table.
   */
  void AddLabel(int iLabel) { m_viLabel.push_back(iLabel); }

  /**
   * @brief Transorm data to OpenCV TrainData
   *
   * Tranform labels and features to opencv matrix
   * and create opencv training data for training classifier.
   */
  cv::Ptr<cv::ml::TrainData> MakeTrainingData(void);

private:    // private method
  /**
    * @brief Add a feature string with its label to label and feature array.
    *
    * @param iLabel The iLabel represents the feature meaning.
    * @param sLineOfString The feature saving in a string.
    */
  void AddSample(int iLabel, const std::string& sLineOfString);
};

} // namespace
#endif // !_MY_SUPERVISED_CLASSIFIER_H_