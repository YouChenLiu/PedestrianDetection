#include "myClassifierBase.h"

namespace Classifier {

void myClassifierBase::SaveFeatures(const std::string& sDstPath) {
  // read a line of feature array
  for (auto& vf : m_vvfFeature) {
    std::string s;
    WriteFeatureToString(vf, s);

    std::stringstream ss(s);
    ss << std::endl;

    // write out the data
    WriteData(ss.str());
  }

  WriteOutFile(sDstPath);
}

void myClassifierBase::LoadFeatures(const std::string& sFeatureFile) {
  // open file
  std::ifstream File(sFeatureFile);

  // string for saving read text
  std::string s;
  while (std::getline(File, s)) {
    AddFeature(s);
  }
}

/**
 * @brief push all features saved in string with specify delim.
 *
 * @param s splited string
 * @param delim spliter character
 * @param vfFeature vector for saving result
 */
void ReadFeaturesFromString(const std::string& s,
                            char delim,
                            std::vector<float>& vfFeature) {
  // tranform string to stream for convenience
  std::stringstream ss(s);

  // string for saving spilt feature
  std::string item;

  // seperate feature string and push to array
  while (std::getline(ss, item, delim)) {
    vfFeature.push_back(stof(item));
  }
}

void myClassifierBase::AddFeature(const std::string& sLineOfFeature) {
  // array for saving feature
  std::vector<float> vfFeature;

  // translate the string to feature
  ReadFeaturesFromString(sLineOfFeature, ',', vfFeature);
  AddFeature(vfFeature);
}

void myClassifierBase::WriteFeatureToString(const std::vector<float>& vfFeature,
                                            std::string& s) const {
  // create a buffer contains write out data
  std::stringstream ss;

  // go throw all value
  for (auto f : vfFeature) {
    // put value to buffer with a common
    ss << f << ',';
  }

  // transfer the buffer data to text string and clip the last common
  s = ss.str();
  s.resize(s.length() - 1);
}

void myClassifierBase::WriteOutFile(const std::string & sDstPath) {
  // open file for writing data
  std::ofstream File(sDstPath);

  // write out the buffer
  File << FileBuffer.str();
  File.close();

  // clear buffer
  FileBuffer.str(std::string());
  FileBuffer.clear();
}

cv::Mat myClassifierBase::ConvertVecToMat(const std::vector<float>& vec) const {
  cv::Mat m(vec);
  cv::transpose(m, m);
  return m;
}

}
