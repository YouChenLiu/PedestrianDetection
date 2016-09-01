#include "myFPPW.h"
#include <set>

namespace Verifier {

const float myFPPI::m_fTHRESHOLD = 0.5f;

myFPPI::myFPPI(const std::string & sGTPath,
               const std::string & sDetectionPath)
    : myVerifierBase(sGTPath, sDetectionPath) {
  Init();
}

void myFPPI::CompareByFrame(int iFrameNum) {
  // vector for ground-gruth and detection result bounding boxes
  std::vector<cv::Rect2i> vrGT, vrDR;
  GetBB(iFrameNum, vrGT, vrDR);
  m_iTotalWindow += vrDR.size();
  // saving groung-truth bounding box index for calculating false negative
  std::set<int> IndexSet;
  // check the number of bounding box
  if (vrGT.size() == 0 || vrDR.size() == 0) {
    if (vrGT.size() == 0) {

    } else if (vrDR.size() == 0) {

    }
    return;
  }
  // increase the total positive
  m_iTotalPositive += vrGT.size();
  // go throw all detection result
  for (const auto& rDR : vrDR) {
    // has matched bounding box
    bool bMatch = false;
    // go throw all ground-truth bounding box
    for (size_t i = 0; i < vrGT.size(); ++i) {
      // take one bounding box
      const auto& rGT = vrGT.at(i);
      // check the overlap ratio
      if (CalOverlap(rDR, rGT) >= 0.5f) {
        // insert the index indicate the matched
        IndexSet.insert(i);
        bMatch = true;
        break;
      }
    }

    if (!bMatch) {
      ++m_iFalsePositive;
    }
  }

  // increase the false negative
  m_iFalseNegative += static_cast<int>(vrGT.size() - IndexSet.size());
}

float myFPPI::GetResult(void) const {
  return static_cast<float>(m_iFalsePositive) / m_iTotalWindow;
}

float myFPPI::GetMissRate(void) const {
  return static_cast<float>(m_iFalseNegative) / m_iTotalPositive;
}

void myFPPI::Init(void) {
  m_iFalseNegative = m_iFalsePositive = 0;
  m_iTotalPositive = m_iTotalWindow = 0;
}

} // namespace