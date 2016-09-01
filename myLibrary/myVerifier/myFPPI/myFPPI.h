/**
 * @file myFPPI.h FPPW definition
 */

#ifndef _MY_FPPI_H_
#define _MY_FPPI_H_

#include "common.h"
#include "myPlugin/myBBReader/myBBReader.h"
#include "../myVerifierBase.h"

namespace Verifier {

/**
 * @brief compare result by false positive per window method
 */
class myFPPI final : public myVerifierBase {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
  int m_iFalsePositive;
  int m_iTotalWindow;
  int m_iFalseNegative;
  int m_iTotalPositive;

  static const float m_fTHRESHOLD;

public:     // public method
  /**
   * @brief default constructor
   */
  myFPPI(void) { Init(); }

  /**
   * @brief create object with window count and file pathes
   *
   * @param iWindowsPerFrame windows count per one frame
   * @param sGTPath XML file path for ground-truth
   * @param sDetectionPath XML file path for xml file want compared
   */
  myFPPI(const std::string& sGTPath,
         const std::string& sDetectionPath);

  virtual void CompareByFrame(int iFrameNum) override;

  virtual float GetResult(void) const override;

  /**
   * @brief Get the miss rate by calculating FPPW
   *
   * @return The miss rate: FN / TP
   */
  float GetMissRate(void) const;

protected:  // protected method

private:    // private method
  void Init(void);
};

} // namespace

#endif // !_MY_FPPI_H_