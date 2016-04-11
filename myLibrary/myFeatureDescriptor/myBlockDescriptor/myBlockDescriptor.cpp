#include "myBlockDescriptor.h"

#ifndef NDEBUG
#   include <iomanip>
#   include <fstream>
#endif

namespace Descriptor {

  const float myBlockDescriptor::m_fUnimportantValue = 1e-20f;

  myBlockDescriptor::myBlockDescriptor(IplImage* pImage, CvSize BlockSize) :
    myBlockDescriptorBase(cv::cvarrToMat(pImage, true),
                          cv::Size2i(BlockSize.width, BlockSize.height)) {
    Init();
  }

  myBlockDescriptor::myBlockDescriptor(
    const cv::Mat& mImage, cv::Size2i BlockSize) :
    myBlockDescriptorBase(mImage, BlockSize) {
    Init();
  }

  myBlockDescriptor::~myBlockDescriptor(void) {}

  void myBlockDescriptor::Init(void) {}

  void myBlockDescriptor::Describe(cv::Point2i Position,
                                    std::vector<float>& vfFeature) const {
    using namespace std;
    vfFeature.clear();

    for (const auto& pExtractor : m_vpoUsedDescriptor) {
      std::vector<float> vfTemp;
      if (auto extractor = dynamic_cast<myHOG*>(pExtractor.get())) {
        extractor->Describe(Position, vfTemp);
      } else if (auto extractor = dynamic_cast<myLBP*>(pExtractor.get())) {
        extractor->Describe(Position, vfTemp);
      }

      auto NormMethod = pExtractor->GetFeatureType();
      if (NormMethod & NORMALIZATION_FLAG) {
        Normalize(NormMethod & NORM_MASK, vfTemp);
      }

      for (auto f : vfTemp) {
        vfFeature.push_back(f);
      }
    }

#   ifndef NDEBUG
    static int iFeatureIndex = 0;
    static ofstream oFeatureFile("Feature.txt");
    if (oFeatureFile.is_open() == true) {
      oFeatureFile.precision(6);
      oFeatureFile << setfill('0') << setw(5) << std::right
        << iFeatureIndex++ << ": ";
      oFeatureFile << setfill(' ');
      int i = 0;
      for (auto feature : vfFeature) {
        stringstream oText;
        oText << i++ << ":" << feature;
        oFeatureFile << setw(12) << std::left << oText.str();
      }
      oFeatureFile << endl;
    }
#   endif
  }

  void myBlockDescriptor::EnableFeature(int iFeature) {
    std::unique_ptr<myDescriptorBase> pExtractor = nullptr;
    switch ((iFeature & FEATURE_MASK) >> FEATURE_OFFSET) {
    case LBP_FEATURE:
      pExtractor = std::make_unique<myLBP>(m_mImage, iFeature, m_BlockSize);
      break;
    case HOG_FEATURE:
      pExtractor = std::make_unique<myHOG>(m_mImage, iFeature, m_BlockSize);
      break;
    default:
      std::cout << "Not valid feature" << std::endl;
      break;
    }

    if (pExtractor != nullptr) {
      m_vpoUsedDescriptor.push_back(std::move(pExtractor));
    }
  }

  void myBlockDescriptor::SetImage(const cv::Mat& mImg) {
    for (auto& base : m_vpoUsedDescriptor) {
      base->SetImage(mImg);
    }
  }

  void myBlockDescriptor::Normalize(int iNormMethod,
                                     std::vector<float>& vfFeature) const {
    float fNormFactor = 0.0f;
    switch (iNormMethod | NORMALIZATION_FLAG) {
    case Feature::L1_NORM:
      fNormFactor = m_fUnimportantValue;
      for (auto fFeature : vfFeature) {
        fNormFactor += fFeature;
      }

      for (auto& fFeature : vfFeature) {
        fFeature = fFeature / fNormFactor * m_iMagnifyingFactor;
      }
      break;
    case Feature::L1_SQRT:
      fNormFactor = m_fUnimportantValue;
      for (auto fFeature : vfFeature) {
        fNormFactor += fFeature;
      }

      for (auto& fFeature : vfFeature) {
        fFeature = cv::sqrt(fFeature / fNormFactor) * m_iMagnifyingFactor;
      }
      break;
    case Feature::L2_NORM:
      fNormFactor = m_fUnimportantValue * m_fUnimportantValue;
      for (auto fFeature : vfFeature) {
        fNormFactor += fFeature * fFeature;
      }
      fNormFactor = cv::sqrt(fNormFactor);

      for (auto& fFeature : vfFeature) {
        fFeature = fFeature / fNormFactor * m_iMagnifyingFactor;
      }
      break;
    default:
      std::cout << "not support normalization method" << std::endl;
      break;
    }
  }

};