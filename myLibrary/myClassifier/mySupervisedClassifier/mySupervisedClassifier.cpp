#include "mySupervisedClassifier.h"
#include <fstream>
#include <sstream>

namespace Classifier {

mySupervisedClassifier::mySupervisedClassifier() {}

mySupervisedClassifier::~mySupervisedClassifier() {}

void mySupervisedClassifier::AddSample(int iLable,
                                       const std::vector<float>& vfFeature) {
  AddFeature(vfFeature);
  AddLabel(iLable);
}

void mySupervisedClassifier::SaveFeatures(const std::string & sDstPath) {
  // write out label and feature table together
  for (size_t i = 0; i < m_viLabel.size(); i++) {
    // use stringstream as buffer
    std::stringstream ss;

    // set flag let system print positive symbol
    ss.setf(std::ios::showpos);

    // write out label and seperate symbol
    ss << m_viLabel.at(i) << ": ";

    // string for represneting feature
    std::string s;

    // tranform feature to string
    WriteFeatureToString(m_vvfFeature.at(i), s);

    // append the feature
    ss << s << std::endl;

    // write data stream
    WriteData(ss.str());
  }

  // write out the cache to file
  WriteOutFile(sDstPath);
}

void mySupervisedClassifier::LoadFeatures(const std::string& sFeatureFile) {
  // open feature file
  std::ifstream File(sFeatureFile);

  // string for saving read text
  std::string s;
  while (std::getline(File, s)) {
    // tranform string to stream for convenience
    std::stringstream ss(s);

    // integer for saving label
    int iLabel = 0;

    // character for remove ':'
    char cSpilter;

    // get label
    ss >> iLabel;

    // remove ';'
    ss >> cSpilter;

    // residue string contains features
    ss >> s;

    // add a sample to table
    AddSample(iLabel, s);
  }

  File.close();
}

void mySupervisedClassifier::Train(void) {
  if ((m_viLabel.size() & m_vvfFeature.size()) == 0) {
    std::cout << "No Labels or Features" << std::endl;
    return;
  }

  m_poClassifier->train(MakeTrainingData());
}
float mySupervisedClassifier::Predict(const cv::Mat & mSample) const {
  if (m_poClassifier == nullptr) return NAN;
  if (!m_poClassifier->isTrained()) return NAN;
  return m_poClassifier->empty() ? NAN : m_poClassifier->predict(mSample);
}
/*
float mySupervisedClassifier::Predict(const std::vector<float>& vfSample) const {
  // get the feature length of sample
  auto iFeatureLength = static_cast<int>(vfSample.size());

  // create matrix for saving feature
  cv::Mat mSample = cv::Mat::zeros(cv::Size2i(iFeatureLength, 1), CV_32FC1);

  // copy feature data to matrix
  for (int x = 0; x < iFeatureLength; x++) {
    mSample.at<float>(0, x) = vfSample.at(x);
  }

  // return the result by calling another method
  return Predict(mSample);
}
*/
bool mySupervisedClassifier::Save(const std::string & sDstPath) const {
  if (!m_poClassifier->empty()) {
    m_poClassifier->save(sDstPath);
    return true;
  } else {
    return false;
  }
}

cv::Ptr<cv::ml::TrainData> mySupervisedClassifier::MakeTrainingData(void) {
  // number of labels
  auto iNumOfLabels = static_cast<int>(m_viLabel.size());

  // matrix for saving labels
  cv::Mat mLabel = cv::Mat::zeros(cv::Size2i(1, iNumOfLabels), CV_32SC1);

  // representation the labels with opencv Mat
  for (int y = 0; y < static_cast<int>(m_viLabel.size()); ++y) {
    mLabel.at<int>(y, 0) = m_viLabel.at(y);
  }

  // clear labels in vector for reducing memory
  m_viLabel.clear();

  // try to deallocate memory
  m_viLabel.shrink_to_fit();

  // number of samples
  auto iNumOfSamples = static_cast<int>(m_vvfFeature.size());

  // the feature length
  auto iFeatureLength = static_cast<int>(m_vvfFeature.at(0).size());

  // matrix for saving features
  cv::Mat mSample = cv::Mat::zeros(cv::Size2i(iFeatureLength, iNumOfSamples),
                                   CV_32FC1);

  // representation the samples with opencv Mat
  for (int y = 0; y < iNumOfLabels; y++) {
    for (int x = 0; x < iFeatureLength; x++) {
      // copy the features from vector to opencv matrix
      mSample.at<float>(y, x) = m_vvfFeature.at(y).at(x);
    }

    // clear the copyed feature for reducing memory
    m_vvfFeature.at(y).clear();
    m_vvfFeature.at(y).shrink_to_fit();
  }

  // clear feature table for reducing memory
  m_vvfFeature.clear();
  m_vvfFeature.shrink_to_fit();

  using namespace cv::ml;
  return TrainData::create(mSample,
                           SampleTypes::ROW_SAMPLE,
                           mLabel);
}

void mySupervisedClassifier::AddSample(int iLable,
                                       const std::string& sLoneOfString) {
  AddLabel(iLable);
  AddFeature(sLoneOfString);
}

};