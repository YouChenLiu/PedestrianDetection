#ifndef _MY_RESULT_VERIFIER_H_
#define _MY_RESULT_VERIFIER_H_

#include "../common.h"
#include "../tinyxml2/tinyxml2.h"
#include <map>
#include <memory>

/**
 * @brief compares the bounding box and computes overlap rate.
 */
class myResultVerifier {
public:     // public attribute
    /**
     * @brief A simple bounding box definition.
     */
    struct myBoundingBox {
        int x;          //!< Left-top point X
        int y;          //!< Left-top point Y
        int width;      //!< Bounding box width
        int height;     //!< Bounfing box height
    };

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
    myResultVerifier(void) { Init(); }

    /**
     * @brief Create by two XML file
     *
     * @param sDetctionResultPath The XML file want to compare.
     * @param sGroundTruthPath The XML file saves the ground-truth.
     */
    myResultVerifier(const std::string sDetctionResultPath,
                     const  std::string sGroundTruthPath);
    ~myResultVerifier(void);

    /**
     * @brief Load two XML file after create.
     *
     * @param sDetctionResultPath The XML file want to compare.
     * @param sGroundTruthPath The XML file saves the ground-truth.
     */
    void LoadXMLFile(const std::string sDetctionResultPath,
                     const  std::string sGroundTruthPath);

    /**
     * @brief Compare two XML file.
     */
    void CompareXMLResult(void);

    /**
     * @brief calculate detection rate.
     */
    float GetDetectionRate(void);
    
#   ifndef NDEBUG
    /**
     * @brief Get true positive score
     */
    float GetTruePositiveScore(void) const { return m_fTruePositiveScore; }
    
    /**
     * @brief Get false positive score
     */
    float GetFalsePositiveScore(void) const { return m_fFalsePositiveScore; }
    
    /**
     * @brief Get true negative score
     */
    float GetTrueNegativeScore(void) const { return m_fTrueNegativeScore; }
    
    /**
     * @brief Get false negative score
     */
    float GetFalseNegativeScore(void) const { return m_fFalseNegativeScore; }
#   endif

protected:  // protected method

private:    // private method
    void Init(void);
    float CalculateOverlapRatio(myBoundingBox Detection,
                                myBoundingBox GroundTruth);
    bool CalculateDetectionRatePerFrame(int iFrameNumber);
    void TakeOutBoundingBox(tinyxml2::XMLElement* poHeader,
                            std::vector<myBoundingBox>& voBoundingBoxes) const;
};

#endif