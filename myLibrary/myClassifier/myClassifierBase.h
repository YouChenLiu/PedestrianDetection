/**
 * @file myClassifierBase.h
 * @brief Classifier base class definition.
 */
#ifndef _MY_CLASSIFIER_BASE_H_
#define _MY_CLASSIFIER_BASE_H_

#include "common.h"
#include <fstream>
#include <sstream>

/**
 * @brief Classifier namespace
 */
namespace Classifier {

/**
 * @brief Base class for my cklassifier library.
 *
 * It contain a vector of vector of float for saving features and
 * some methods for accessing it such as load and save features.
 * It also contain some methods for calssifier like train and predict.
 * If you want to design your classifer, you should inherit derived class
 * superviesed or unsupervised class.
 * Do not modify this class or modify it carefully, please.
 */
class myClassifierBase {
public:     // public attribute

protected:  // protected attribute
  /// the array for saving features
  std::vector<std::vector<float>> m_vvfFeature;

private:    // private attribute
  /// stream for caching write out data
  std::stringstream FileBuffer;

public:     // public method
  /**
   * @brief Default Constructor.
   */
  myClassifierBase(void) {};
  virtual ~myClassifierBase(void) {};

  /**
   * @brief Training the classifier by contaning samples.
   */
  virtual void Train(void) = 0;

  /**
   * @brief Predict response for the provided sample
   *  encapsulated in opencv matrix.
   *
   * @param mSample Features saved in Mat
   * @return The classification result.
   * @retval NAN The classifier is empty.
   */
  virtual float Predict(const cv::Mat& mSample) const = 0;

  /**
   * @brief Predict response for the provided sample encapsulated in
   *  std::vector.
   *
   * @param vfSample Features saved in vector.
   * @return The classifcation result.
   * @retval NAN The classifier is empty.
   */
  virtual float Predict(const std::vector<float>& vfSample) const {
    return Predict(ConvertVecToMat(vfSample));
  }

  /**
   * @brief Save the trained classifier to an xml file.
   *
   * @param sDstPath The destination file path.
   * The saved file should be XML file.
   * @return Saving successfil or not.
   * @retval True Saving model is successful.
   * @retval False Saving model is not successful.
   */
  virtual bool Save(const std::string& sDstPath) const = 0;

  /**
   * @brief Save features in table to csv file.
   *
   * @param sDstPath The destination file path.
   */
  virtual void SaveFeatures(const std::string& sDstPath);

  /**
   * @brief Create classifier from file.
   *
   * @param sModelFile The file path for reading
   */
  virtual void Load(const std::string& sModelFile) = 0;

  /**
   * @brief Create classifier from feature file.
   *
   * @param sFeatureFile The file path for reading.
   */
  virtual void LoadFeatures(const std::string& sFeatureFile);

protected:  // protected method
  /**
   * @brief Add a sample to array.
   *
   * @param vfNewFeature New feature want to add to table.
   */
  void AddFeature(const std::vector<float>& vfNewFeature) {
    m_vvfFeature.push_back(vfNewFeature);
  }

  /**
   * @brief Add a line features saved in string
   * which uses common for seperating.
   *
   * @param sLineOfFeature Features saved in string.
   */
  void AddFeature(const std::string& sLineOfFeature);

  /**
   * @brief Write data to cache.
   *
   * @param sData The data will write to cache.
   */
  void WriteData(const std::string& sData) {
    FileBuffer << sData;
  }

  /**
   * @brief Tranform feature vector to string.
   *
   * @param vfFeature The feature will save to string.
   * @param s The string saved the features with common.
   */
  void WriteFeatureToString(const std::vector<float>& vfFeature,
                            std::string& s) const;

  /**
   * @brief Write out the caching data to text file.
   *
   * @param sDstPath The destination path for saving data.
   */
  void WriteOutFile(const std::string& sDstPath);

  /**
   * @brief convert the feature vector in std::vector to cv::Mat format
   *
   * @param vec The vector want to convert
   * @return A single row cv::Mat
   */
  cv::Mat ConvertVecToMat(const std::vector<float>& vec) const;

private:    // private method

};

}
#endif // !_MY_CLASSIFIER_BASE_H_