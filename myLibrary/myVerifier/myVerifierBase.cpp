#include "myVerifierBase.h"

namespace Verifier {

myVerifierBase::myVerifierBase(const std::string & sGTPath,
                               const std::string & sResultPath) {
  m_oGroundTruthBB.LoadXML(sGTPath);
  m_oDetectionBB.LoadXML(sResultPath);
}

void myVerifierBase::LoadGroundTruth(const std::string & sGTPath) {
  m_oGroundTruthBB.LoadXML(sGTPath);
}

void myVerifierBase::LoadDetection(const std::string & sDetectionPath) {
  m_oDetectionBB.LoadXML(sDetectionPath);
}

void myVerifierBase::GetBB(int iFrameNum,
                           std::vector<cv::Rect2i>& vrGroundTruth,
                           std::vector<cv::Rect2i>& vrDetection) const {
  m_oGroundTruthBB.TakeOutBBbyFrame(iFrameNum, vrGroundTruth);
  m_oDetectionBB.TakeOutBBbyFrame(iFrameNum, vrDetection);
}

void myVerifierBase::CompareByFrames(int iStart, int iEnd) {
  for (int i = iStart; i <= iEnd; ++i) {
    CompareByFrame(i);
  }
}

void myVerifierBase::CompareAllFrames(void) {
  auto iFirstFrame = m_oDetectionBB.GetFirstFrameNum();
  auto iEndFrame = m_oDetectionBB.GetEndFrameNum();
  CompareByFrames(iFirstFrame, iEndFrame);
}

void myVerifierBase::Init(void) {}

}