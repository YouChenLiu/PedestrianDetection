#include "myFPPW.h"

namespace Verifier {

const float myFPPW::m_fTHRESHOLD = 0.5f;

myFPPW::myFPPW(int iWindowsPerFrame,
               const std::string & sGTPath,
               const std::string & sDetectionPath)
    : myVerifierBase(sGTPath, sDetectionPath) {
  m_iWindowsPerFrame = iWindowsPerFrame;
}

void myFPPW::CompareByFrame(int iFrameNum) {
  m_iTotalWindow += m_iWindowsPerFrame;
  std::vector<cv::Rect2i> vrGT, vrDR;
  GetBB(iFrameNum, vrGT, vrDR);

  if (vrGT.size() == 0 || vrDR.size() == 0) {
    if (vrGT.size() == 0) {

    } else if (vrDR.size() == 0) {

    }
    return;
  }

  for (const auto& rDR : vrDR) {
    bool bMatch = false;
    for (const auto& rGT : vrGT) {
      if (CalOverlap(rDR, rGT) >= 0.5f) {
        bMatch = true;
        break;
      }
    }

    if (!bMatch) {
      ++m_iFalsePositive;
    }
  }
}

float myFPPW::GetResult(void) const {
  return static_cast<float>(m_iFalsePositive) / m_iTotalWindow;
}

void myFPPW::Init(void) {
  m_iWindowsPerFrame = -1;
}

} // namespace