#include "myFeatureExtractor.h"

#ifndef NDEBUG
#   include <iomanip>
#   include <fstream>
#endif

myFeatureExtractor::myFeatureExtractor(IplImage* pImage, CvSize BlockSize) :
    myBlockBasedExtractor(cv::cvarrToMat(pImage, true),
                          cv::Size2i(BlockSize.width, BlockSize.height)){
    Init();
}

myFeatureExtractor::myFeatureExtractor(
    const cv::Mat& mImage, cv::Size2i BlockSize) :
    myBlockBasedExtractor(mImage,BlockSize) {
    Init();
}

myFeatureExtractor::~myFeatureExtractor(void) {}

void myFeatureExtractor::Init(void) {}

void myFeatureExtractor::Describe(cv::Point2i Position,
                                  std::vector<float>& vfFeature) const {
    using namespace std;
    vfFeature.clear();
    
    for (const auto& pExtractor : m_vpoUsedExtractor) {
        std::vector<float> vfTemp;
        if (auto extractor = dynamic_cast<myHOG*>(pExtractor.get())) {
            extractor->Describe(Position, vfTemp);
        } else if (auto extractor = dynamic_cast<myLBP*>(pExtractor.get())) {
            extractor->Describe(Position, vfTemp);
        }
        for (auto f : vfTemp) {
            vfFeature.push_back(f);
        }
    }

#   ifndef NDEBUG
    static int iFeatureIndex = 0;
    static ofstream oFeatureFile("Feature.txt");
    if (oFeatureFile.is_open() == true) {
        oFeatureFile.precision(6);
        oFeatureFile << setfill('0') << setw(5) << std::right
                     << iFeatureIndex++ << ": ";
        oFeatureFile << setfill(' ');
        int i = 0;
        for (auto feature : vfFeature) {
            stringstream oText;
            oText << i++ << ":" << feature;
            oFeatureFile << setw(12) <<  std::left << oText.str();
        }
        oFeatureFile << endl;
    }
#   endif
}

void myFeatureExtractor::EnableFeature(int iFeature) {
    std::unique_ptr<myExtractorBase> pExtractor = nullptr;
    switch (iFeature / 10) {
    case 0: // LBP feature 0 <= iFeature <= 5
        pExtractor = std::make_unique<myLBP>(m_mImage, iFeature, m_BlockSize);
        break;
    case 1: // HOG feature 10 <= iFeature <= 14
        pExtractor = std::make_unique<myHOG>(m_mImage, iFeature, m_BlockSize);
        break;
    default:
        std::cout << "Not valid feature" << std::endl;
        break;
    }

    m_vpoUsedExtractor.push_back(std::move(pExtractor));
}