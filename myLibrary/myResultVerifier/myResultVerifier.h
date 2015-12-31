#ifndef _MY_RESULT_VERIFIER_H_
#define _MY_RESULT_VERIFIER_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "../myXMLLabel/myXMLLabel.h"
#include "../tinyxml2/tinyxml2.h"

class myResultVerifier {
public:
    struct myBoundingBox {
        int x;
        int y;
        int width;
        int height;
    };

public:
    myResultVerifier(void) { Init(); }
    myResultVerifier(const std::string sDetctionResultPath, const  std::string sGroundTruthPath);
    ~myResultVerifier(void);

    void LoadXMLFile(const std::string sDetctionResultPath, const  std::string sGroundTruthPath);
    void CompareXMLResult(void);
    float GetDetectionRate(void);
    
#   ifndef NDEBUG
    float GetTruePositiveScore(void) const { return m_fTruePositiveScore; }
    float GetFalsePositiveScore(void) const { return m_fFalsePositiveScore; }
    float GetTrueNegativeScore(void) const { return m_fTrueNegativeScore; }
    float GetFalseNegativeScore(void) const { return m_fFalseNegativeScore; }
#   endif

private:
    float m_fTruePositiveScore;
    float m_fFalsePositiveScore;
    float m_fTrueNegativeScore;
    float m_fFalseNegativeScore;
    int m_iTotalFrames;
    std::unique_ptr<tinyxml2::XMLDocument> m_poDetectionResultXMLFile;
    std::unique_ptr<tinyxml2::XMLDocument> m_poGroundTruthXMLFile;

private:
    void Init(void);
    float CalculateOverlapRatio(myBoundingBox Detection, myBoundingBox GroundTruth);
    bool CalculateDetectionRatePerFrame(int iFrameNumber);
    void TakeOutBoundingBox(tinyxml2::XMLElement* poHeader, std::vector<myBoundingBox>& voBoundingBoxes) const;
};

#endif