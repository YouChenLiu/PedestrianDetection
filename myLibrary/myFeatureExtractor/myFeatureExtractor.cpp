#include "myFeatureExtractor.h"

myFeatureExtractor::myFeatureExtractor(IplImage* pImage, CvSize BlockSize) {
    Init();
    m_mImage = cv::cvarrToMat(pImage, true);
    m_BlockSize = cv::Size2i(BlockSize.width, BlockSize.height);
}

myFeatureExtractor::myFeatureExtractor(cv::Mat& mImage, cv::Size2i BlockSize) {
    Init();
    m_mImage = mImage;
    m_BlockSize = BlockSize;
}

myFeatureExtractor::~myFeatureExtractor(void) {
    for (auto p : m_vpoUsedExtractor) {
        delete p;
    }
}

void myFeatureExtractor::Init(void) {
    m_BlockSize = cv::Size2i(0, 0);
    m_mImage = cv::Mat();
}

void myFeatureExtractor::Describe(cv::Point2i Position, std::vector<float>& vfFeature) const {
    using namespace std;
    vfFeature.clear();
    
    for (const auto pExtractor : m_vpoUsedExtractor) {
        std::vector<float> vfTemp;
        if (auto extractor = dynamic_cast<myHOG*>(pExtractor)) {
            extractor->Describe(Position, vfTemp);
        } else if (auto extractor = dynamic_cast<myLBP*>(pExtractor)) {
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
        oFeatureFile << setfill('0') << setw(5) << std::right << iFeatureIndex++ << ": ";
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
    switch (iFeature / 10) {
    case 0: // LBP feature 0 <= iFeature <= 5
        m_vpoUsedExtractor.push_back(new myLBP(m_mImage, iFeature, m_BlockSize));
        break;
    case 1: // HOG feature 10 <= iFeature <= 14
        m_vpoUsedExtractor.push_back(new myHOG(m_mImage, iFeature, m_BlockSize));
        break;
    default:
        std::cout << "Not valid feature" << std::endl;
        break;
    }
}