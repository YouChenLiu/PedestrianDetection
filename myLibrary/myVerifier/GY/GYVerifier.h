/**
 * @file GYVerifier.h
 * @brief Gyun-Yuan's result verifier class definition.
 */

#ifndef _MY_RESULT_VERIFIER_H_
#define _MY_RESULT_VERIFIER_H_

#include "common.h"
#include "../myVerifierBase.h"
#include "tinyxml2/tinyxml2.h"
#include <memory>

namespace Verifier {

/**
 * @brief compares the bounding box and computes overlap rate.
 */
class GYVerifier : public myVerifierBase {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
  float m_fTruePositiveScore;
  float m_fFalsePositiveScore;
  float m_fTrueNegativeScore;
  float m_fFalseNegativeScore;
  int m_iTotalFrames;
  std::unique_ptr<tinyxml2::XMLDocument> m_poDetectionResultXMLFile;
  std::unique_ptr<tinyxml2::XMLDocument> m_poGroundTruthXMLFile;

public:     // public method
  /**
   * @brief Default constructor.
   */
  GYVerifier(void) { Init(); }

  /**
   * @brief Create by two XML file
   *
   * @param sDetctionResultPath The XML file want to compare.
   * @param sGroundTruthPath The XML file saves the ground-truth.
   */
  GYVerifier(const std::string sDetctionResultPath,
                   const std::string sGroundTruthPath);
  ~GYVerifier(void);

  /**
   * @brief Load two XML file after create.
   *
   * @param sDetctionResultPath The XML file want to compare.
   * @param sGroundTruthPath The XML file saves the ground-truth.
   */
  void LoadXMLFile(const std::string sDetctionResultPath,
                   const std::string sGroundTruthPath);

  /**
   * @brief Compare two XML file.
   */
  void CompareXMLResult(void);

  /**
   * @brief calculate detection rate.
   */
  float GetDetectionRate(void);

protected:  // protected method

private:    // private method
  void Init(void);
  float CalculateOverlapRatio(cv::Rect2i Detection, cv::Rect2i GroundTruth);
  bool CalculateDetectionRatePerFrame(int iFrameNumber);
  void TakeOutBoundingBox(tinyxml2::XMLElement* poHeader,
                          std::vector<cv::Rect2i>& voBoundingBoxes) const;
};  // class

} // namespase
#endif