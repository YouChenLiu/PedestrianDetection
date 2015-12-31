#include "myResultVerifier.h"

myResultVerifier::myResultVerifier(const std::string sDetctionResult, const std::string sGroundTruth) {
    Init();
    LoadXMLFile(sDetctionResult, sGroundTruth);
}

myResultVerifier::~myResultVerifier(void) {}

void myResultVerifier::Init(void) {
    m_poDetectionResultXMLFile = std::make_unique<tinyxml2::XMLDocument>();
    m_poGroundTruthXMLFile = std::make_unique<tinyxml2::XMLDocument>();
    m_fTruePositiveScore = m_fFalsePositiveScore = m_fTrueNegativeScore = m_fFalseNegativeScore = 0.0f;
    m_iTotalFrames = 0;
}

void myResultVerifier::LoadXMLFile(const std::string sDetctionResult, const std::string sGroundTruth) {
    m_poDetectionResultXMLFile->LoadFile(sDetctionResult.c_str());
    m_poGroundTruthXMLFile->LoadFile(sGroundTruth.c_str());
}

float myResultVerifier::GetDetectionRate(void) {
    float fDetectionRate = 0.0f;

    if ((m_fTruePositiveScore + m_fFalsePositiveScore + m_fTrueNegativeScore + m_fFalseNegativeScore) == 0.0f) {
        CompareXMLResult();
    }

    fDetectionRate = m_fTruePositiveScore / (m_fTruePositiveScore + m_fFalseNegativeScore);

    return fDetectionRate;
}

void myResultVerifier::CompareXMLResult(void) {
    while (CalculateDetectionRatePerFrame(m_iTotalFrames) == true) {
        ++m_iTotalFrames;
    }

    m_iTotalFrames -= 1;

#   ifndef NDEBUG
    std::cout << "Total Frames = " << m_iTotalFrames << std::endl;
    std::cout << "TP Score = " << m_fTruePositiveScore << std::endl;
    std::cout << "FP Score = " << m_fFalsePositiveScore << std::endl;
    std::cout << "TN Score = " << m_fTrueNegativeScore << std::endl;
    std::cout << "FN Score = " << m_fFalseNegativeScore << std::endl;
#   endif
}

bool myResultVerifier::CalculateDetectionRatePerFrame(int iFrameNumber) {
    using namespace tinyxml2;

    XMLElement* poDetectionDataSet = m_poDetectionResultXMLFile->RootElement()->FirstChildElement(myXMLLabel::GetTagString(myXMLLabel::Tags::DATA_SET).c_str());
    XMLElement* poGroundTruthDataSet = m_poGroundTruthXMLFile->RootElement()->FirstChildElement(myXMLLabel::GetTagString(myXMLLabel::Tags::DATA_SET).c_str());

    XMLElement* poDetectionHeader = poDetectionDataSet->FirstChildElement(myXMLLabel::GetTagString(myXMLLabel::Tags::HEADER).c_str());
    while (poDetectionHeader != nullptr) {
        int iFrame = poDetectionHeader->IntAttribute(myXMLLabel::GetAttributeString(myXMLLabel::Attributes::FRAME_NUMBER).c_str());
        if (iFrame >= iFrameNumber) {
            if (iFrame != iFrameNumber) {
                poDetectionHeader = nullptr;
            }
            break;
        }
        poDetectionHeader = poDetectionHeader->NextSiblingElement();
    }
    
    XMLElement* poGroundTruthHeader = poGroundTruthDataSet->FirstChildElement(myXMLLabel::GetTagString(myXMLLabel::Tags::HEADER).c_str());
    while (poGroundTruthHeader != nullptr) {
        int iFrame = poGroundTruthHeader->IntAttribute(myXMLLabel::GetAttributeString(myXMLLabel::Attributes::FRAME_NUMBER).c_str());
        if (iFrame >= iFrameNumber) {
            if (iFrame != iFrameNumber) {
                poDetectionHeader = nullptr;
            }
            break;
        }
        poGroundTruthHeader = poGroundTruthHeader->NextSiblingElement();
    }

    if ((poDetectionHeader == nullptr) || (poGroundTruthHeader == nullptr)) {
        return false;
    }

    std::vector<myBoundingBox> voDetectionBoxes;
    TakeOutBoundingBox(poDetectionHeader, voDetectionBoxes);
    std::vector<myBoundingBox> voGroundTruthBoxes;
    TakeOutBoundingBox(poGroundTruthHeader, voGroundTruthBoxes);

    if (static_cast<int>(voDetectionBoxes.size()) == 0) {
        //if one of xml files has no corresponding records
        return true;
    }

    if (static_cast<int>(voGroundTruthBoxes.size()) == 0) {
        return true;
    }

    for (int iDetectionIndex = 0; iDetectionIndex < static_cast<int>(voDetectionBoxes.size()); ++iDetectionIndex) {
        myBoundingBox DetectionBox = voDetectionBoxes.at(iDetectionIndex);
        float fMaxOverlapRatio = 0.0f;
        for (int iGTIndex = 0; iGTIndex < static_cast<int>(voGroundTruthBoxes.size()); ++iGTIndex) {
            myBoundingBox GTBox = voGroundTruthBoxes.at(iGTIndex);
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

inline
void myResultVerifier::TakeOutBoundingBox(tinyxml2::XMLElement* poHeader, std::vector<myBoundingBox>& voBoundingBoxes) const {
    using namespace tinyxml2;

    voBoundingBoxes.clear();

    XMLElement* poRecord = poHeader->FirstChildElement(myXMLLabel::GetTagString(myXMLLabel::Tags::RECORD).c_str());
    while (poRecord != nullptr) {
        const std::string s = myXMLLabel::GetTagString(myXMLLabel::Tags::START_POINT);
        XMLElement* Element = poRecord->FirstChildElement(myXMLLabel::GetTagString(myXMLLabel::Tags::START_POINT).c_str());
        const std::string sStartPoint = Element->GetText();
        const int iCommaIndex = static_cast<int>(sStartPoint.find(", "));
        const int x = stoi(sStartPoint.substr(0, iCommaIndex));
        const int y = stoi(sStartPoint.substr(iCommaIndex + 1, sStartPoint.size()));
        const int iWidth = std::stoi(poRecord->FirstChildElement(myXMLLabel::GetTagString(myXMLLabel::Tags::WIDTH).c_str())->GetText());
        const int iHeight = std::stoi(poRecord->FirstChildElement(myXMLLabel::GetTagString(myXMLLabel::Tags::HEIGHT).c_str())->GetText());

        const myBoundingBox box = { x, y, iWidth, iHeight };
        voBoundingBoxes.push_back(box);
        poRecord = poRecord->NextSiblingElement();
    }
}

inline
float myResultVerifier::CalculateOverlapRatio(myResultVerifier::myBoundingBox Detection, myResultVerifier::myBoundingBox GroundTruth) {
    const int iDetectionArea = Detection.height * Detection.width;
    const int iGroundTruthArea = GroundTruth.height * GroundTruth.width;

    int iOverlapArea = 0;
    for (int y = Detection.y; y < Detection.y + Detection.height; ++y) {
        for (int x = Detection.x; x < Detection.x + Detection.width; ++x) {
            if ((GroundTruth.y <= y && y < GroundTruth.y + GroundTruth.height) && 
                (GroundTruth.x <= x && x < GroundTruth.x + GroundTruth.width)) {
                ++iOverlapArea;
            }
        }
    }

    return (float)iOverlapArea / (float)(iDetectionArea + iGroundTruthArea - iOverlapArea);
}