/**
 * @file myFPPW.h FPPW definition
 */

#ifndef _MY_FPPW_H_
#define _MY_FPPW_H_

#include "common.h"
#include "myPlugin/myBBReader/myBBReader.h"
#include "../myVerifierBase.h"

namespace Verifier {

/**
 * @brief compare result by false positive per window method
 */
class myFPPW final : public myVerifierBase {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
  int m_iWindowsPerFrame;
  int m_iFalsePositive;
  int m_iTotalWindow;

  static const float m_fTHRESHOLD;

public:     // public method
  /**
   * @brief default constructor
   */
  myFPPW(void) { Init(); }

  /**
   * @brief create object with window count and file pathes
   *
   * @param iWindowsPerFrame windows count per one frame
   * @param sGTPath XML file path for ground-truth
   * @param sDetectionPath XML file path for xml file want compared
   */
  myFPPW(int iWindowsPerFrame,
         const std::string& sGTPath,
         const std::string& sDetectionPath);

  virtual void CompareByFrame(int iFrameNum) override;

  virtual float GetResult(void) const override;

protected:  // protected method

private:    // private method
  void Init(void);
};

} // namespace

#endif // !_MY_FPPW_H_