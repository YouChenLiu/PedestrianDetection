#include "GYVerifier.h"

namespace Verifier {

GYVerifier::GYVerifier(const std::string sDetctionResult, const std::string sGroundTruth) {
  Init();
  LoadXMLFile(sDetctionResult, sGroundTruth);
}

GYVerifier::~GYVerifier(void) {}

void GYVerifier::Init(void) {
  m_poDetectionResultXMLFile = std::make_unique<tinyxml2::XMLDocument>();
  m_poGroundTruthXMLFile = std::make_unique<tinyxml2::XMLDocument>();
  m_fTruePositiveScore = m_fFalsePositiveScore = m_fTrueNegativeScore = m_fFalseNegativeScore = 0.0f;
  m_iTotalFrames = 0;
}

void GYVerifier::LoadXMLFile(const std::string sDetctionResult, const std::string sGroundTruth) {
  m_poDetectionResultXMLFile->LoadFile(sDetctionResult.c_str());
  m_poGroundTruthXMLFile->LoadFile(sGroundTruth.c_str());
}

float GYVerifier::GetDetectionRate(void) {
  float fDetectionRate = 0.0f;

  if ((m_fTruePositiveScore + m_fFalsePositiveScore + m_fTrueNegativeScore + m_fFalseNegativeScore) == 0.0f) {
    CompareXMLResult();
  }

  fDetectionRate = m_fTruePositiveScore / (m_fTruePositiveScore + m_fFalseNegativeScore);

  return fDetectionRate;
}

void GYVerifier::CompareXMLResult(void) {
  while (CalculateDetectionRatePerFrame(m_iTotalFrames) == true) {
    ++m_iTotalFrames;
  }

  m_iTotalFrames -= 1;

# ifndef NDEBUG
  std::cout << "Total Frames = " << m_iTotalFrames << std::endl;
  std::cout << "TP Score = " << m_fTruePositiveScore << std::endl;
  std::cout << "FP Score = " << m_fFalsePositiveScore << std::endl;
  std::cout << "TN Score = " << m_fTrueNegativeScore << std::endl;
  std::cout << "FN Score = " << m_fFalseNegativeScore << std::endl;
# endif
}

bool GYVerifier::CalculateDetectionRatePerFrame(int iFrameNumber) {
  using namespace tinyxml2;
  using namespace Plugin;
  auto poDetectionData = m_poDetectionResultXMLFile->RootElement()->FirstChildElement(GetLabel(Tags::DATA_SET).c_str());
  auto poGTData = m_poGroundTruthXMLFile->RootElement()->FirstChildElement(GetLabel(Tags::DATA_SET).c_str());

  auto poDetectionHeader = poDetectionData->FirstChildElement(GetLabel(Tags::HEADER).c_str());
  while (poDetectionHeader != nullptr) {
    int iFrame = poDetectionHeader->IntAttribute(GetLabel(Attributes::FRAME_NUMBER).c_str());
    if (iFrame >= iFrameNumber) {
      if (iFrame != iFrameNumber) {
        poDetectionHeader = nullptr;
      }
      break;
    }
    poDetectionHeader = poDetectionHeader->NextSiblingElement();
  }

  auto poGTHeader = poGTData->FirstChildElement(GetLabel(Tags::HEADER).c_str());
  while (poGTHeader != nullptr) {
    int iFrame = poGTHeader->IntAttribute(GetLabel(Attributes::FRAME_NUMBER).c_str());
    if (iFrame >= iFrameNumber) {
      if (iFrame != iFrameNumber) {
        poDetectionHeader = nullptr;
      }
      break;
    }
    poGTHeader = poGTHeader->NextSiblingElement();
  }

  if ((poDetectionHeader == nullptr) || (poGTHeader == nullptr)) {
    return false;
  }

  std::vector<cv::Rect2i> voDetectionBoxes;
  TakeOutBoundingBox(poDetectionHeader, voDetectionBoxes);
  std::vector<cv::Rect2i> voGroundTruthBoxes;
  TakeOutBoundingBox(poGTHeader, voGroundTruthBoxes);

  if (static_cast<int>(voDetectionBoxes.size()) == 0) {
    //if one of xml files has no corresponding records
    return true;
  }

  if (static_cast<int>(voGroundTruthBoxes.size()) == 0) {
    return true;
  }

  for (int iDetectionIndex = 0; iDetectionIndex < static_cast<int>(voDetectionBoxes.size()); ++iDetectionIndex) {
    auto DetectionBox = voDetectionBoxes.at(iDetectionIndex);
    float fMaxOverlapRatio = 0.0f;
    for (int iGTIndex = 0; iGTIndex < static_cast<int>(voGroundTruthBoxes.size()); ++iGTIndex) {
      auto GTBox = voGroundTruthBoxes.at(iGTIndex);
      float fOverlapRatio = CalculateOverlapRatio(DetectionBox, GTBox);
      if (fOverlapRatio > fMaxOverlapRatio) {
        fMaxOverlapRatio = fOverlapRatio;
      }
    }
    m_fTruePositiveScore += fMaxOverlapRatio;
    m_fFalsePositiveScore += 1 - fMaxOverlapRatio;
    m_fFalseNegativeScore += 1 - fMaxOverlapRatio;
  }

  return true;
}

void GYVerifier::TakeOutBoundingBox(tinyxml2::XMLElement* poHeader, std::vector<cv::Rect2i>& voBoundingBoxes) const {
  using namespace tinyxml2;
  using namespace Plugin;
  voBoundingBoxes.clear();

  XMLElement* poRecord = poHeader->FirstChildElement(GetLabel(Tags::RECORD).c_str());
  while (poRecord != nullptr) {
    const std::string s = GetLabel(Tags::START_POINT);
    XMLElement* Element = poRecord->FirstChildElement(GetLabel(Tags::START_POINT).c_str());
    const std::string sStartPoint = Element->GetText();
    const int iCommaIndex = static_cast<int>(sStartPoint.find(", "));
    const int x = stoi(sStartPoint.substr(0, iCommaIndex));
    const int y = stoi(sStartPoint.substr(iCommaIndex + 1, sStartPoint.size()));
    const int iWidth = std::stoi(poRecord->FirstChildElement(GetLabel(Tags::WIDTH).c_str())->GetText());
    const int iHeight = std::stoi(poRecord->FirstChildElement(GetLabel(Tags::HEIGHT).c_str())->GetText());

    const cv::Rect2i box(x, y, iWidth, iHeight);
    voBoundingBoxes.push_back(box);
    poRecord = poRecord->NextSiblingElement();
  }
}

float GYVerifier::CalculateOverlapRatio(cv::Rect2i Detection, cv::Rect2i GroundTruth) {
  const auto iDetectionArea = Detection.area();
  const auto iGroundTruthArea = GroundTruth.area();

  int iOverlapArea = 0;
  for (int y = Detection.y; y < Detection.y + Detection.height; ++y) {
    for (int x = Detection.x; x < Detection.x + Detection.width; ++x) {
      cv::Point2i pt(x, y);
      if (GroundTruth.contains(pt)) {
        ++iOverlapArea;
      }
    }
  }

  return static_cast<float>(iOverlapArea) / (iDetectionArea + iGroundTruthArea - iOverlapArea);
}

} // namespase