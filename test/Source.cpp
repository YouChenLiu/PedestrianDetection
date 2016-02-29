#include "../myLibrary/myClassifier/mySVM/mySVM.h"
#include "../myLibrary/myModelCollector/myModelCollector.h"
#include "../myLibrary/myFeatureExtractor/myFeatureExtractor.h"
#include "../myLibrary/myModelIndexer/myLBPIndexer/myLBPIndexer.h"
#include "../myLibrary/myImageSequence/myImageSequence.h"
#include <iomanip>

int main(void) {
    const cv::Size2i ImgSize(64, 128);
    const cv::Size2i BlockSize(8, 8);
    const int iCollectorCount = (((ImgSize.height - 2 * BlockSize.height) / 8) *
                                 ((ImgSize.width - 2 * BlockSize.width) / 8));
    const std::string sRootPath = "D:/Backup/Thermal/night/";
    const std::string sSubFolder = "Morning-Noon/";

    std::vector<myModelCollector> voCollector(iCollectorCount);
    for (auto& o : voCollector) {
        o.Resize(59);
    }

    myLBPIndexer oIndexr(BlockSize);

    myImageSequence oPosReader(sRootPath + "Positive/", "", "bmp", false);
    oPosReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    myImageSequence oNegReader(sRootPath + "Negative/", "", "bmp", false);
    oNegReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    
    cv::Mat mPosImg;
    cv::Mat mNegImg;
    /*
    std::cout << "Reading Positive Samples" << std::endl;
    while (oPosReader >> mPosImg) {
        myFeatureExtractor oExtractor(mPosImg, BlockSize);
        oExtractor.EnableFeature(myFeatureExtractor::Features::HOG_WITH_L2_NORM);
        oExtractor.EnableFeature(myFeatureExtractor::Features::LBP_8_1_UNIFORM);
        
        for (int y = BlockSize.height, iPos = 0; y < mPosImg.rows - BlockSize.height; y += BlockSize.height) {
            for (int x = BlockSize.width; x < mPosImg.cols - BlockSize.width; x += BlockSize.width, ++iPos) {
                std::vector<float> vfFeature;
                cv::Point2i Position(x, y);
                oExtractor.Describe(Position, vfFeature);
                auto iIndex = oIndexr.GetBinNumber(mPosImg, Position);
                voCollector.at(iPos).AddSample(iIndex, +1, vfFeature);
            }
        }
    }
    std::cout << oPosReader.GetSequenceNumber() - 1 << "Samples" << std::endl;

    std::cout << "Reading Negative Samples" << std::endl;
    while (oNegReader >> mNegImg) {
        myFeatureExtractor oExtractor(mNegImg, BlockSize);
        oExtractor.EnableFeature(myFeatureExtractor::Features::HOG_WITH_L2_NORM);
        oExtractor.EnableFeature(myFeatureExtractor::Features::LBP_8_1_UNIFORM);

        for (int y = BlockSize.height, iPos = 0; y < mNegImg.rows - BlockSize.height; y += BlockSize.height) {
            for (int x = BlockSize.width; x < mNegImg.cols - BlockSize.width; x += BlockSize.width, ++iPos) {
                std::vector<float> vfFeature;
                cv::Point2i Position(x, y);
                oExtractor.Describe(Position, vfFeature);
                auto iIndex = oIndexr.GetBinNumber(mNegImg, Position);
                voCollector.at(iPos).AddSample(iIndex, -1, vfFeature);
            }
        }
    }
    std::cout << oNegReader.GetSequenceNumber() - 1 << "Samples" << std::endl;

    std::ofstream FeatureList("feature.txt");
    FeatureList << voCollector.size() << std::endl;
    for (std::size_t i = 0; i < voCollector.size(); ++i) {
        FeatureList << voCollector.at(i).SaveFeatures("Features") << std::endl;
    }
    
    std::ofstream ModelList("models.txt");
    ModelList << voCollector.size() << std::endl;
    for (std::size_t i = 0; i < voCollector.size(); ++i) {
        voCollector.at(i).TrainModels();
        ModelList << voCollector.at(i).SaveModels("Models") << std::endl;
    }
    */

    std::ifstream ModelList("models.txt");
    int iModelsCount = 0;
    ModelList >> iModelsCount;
    for (std::size_t i = 0; i < iModelsCount; ++i) {
        std::string sPath;
        ModelList >> sPath;
        voCollector.at(i).LoadModels(sPath);
    }

    std::cout << "Testing Positive Samples" << std::endl;
    myImageSequence oPosWriter(sSubFolder + "Positive/", "", "jpg");
    oPosWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    while (oPosReader >> mPosImg) {
        myFeatureExtractor oExtractor(mPosImg, BlockSize);
        oExtractor.EnableFeature(myFeatureExtractor::Features::HOG_WITH_L2_NORM);
        oExtractor.EnableFeature(myFeatureExtractor::Features::LBP_8_1_UNIFORM);

        //cv::Mat mPosResult = cv::Mat::ones(mPosImg.size(), CV_8UC1) * 127;
        cv::Mat mPosResult = cv::Mat::ones(cv::Size2i(ImgSize.width * 2, ImgSize.height), CV_8UC1) * 127;
        for (int y = BlockSize.height, iPos = 0; y < mPosImg.rows - BlockSize.height; y += BlockSize.height) {
            for (int x = BlockSize.width; x < mPosImg.cols - BlockSize.width; x += BlockSize.width, ++iPos) {
                std::vector<float> vfFeature;
                cv::Point2i Position(x, y);
                oExtractor.Describe(Position, vfFeature);
                auto iIndex = oIndexr.GetBinNumber(mPosImg, Position);
                auto fResult = voCollector.at(iPos).Predict(iIndex, vfFeature);
                unsigned char uc = (fResult == +1) ? 255u : (fResult == -1) ? 0u : 127u;
                mPosResult(cv::Rect2i(cv::Point2i(64 + x, y), BlockSize)) = cv::Scalar::all(uc);
            }
        }
        mPosImg.copyTo(mPosResult(cv::Rect2i(cv::Point2i(0, 0), cv::Size2i(64, 128))));
        oPosWriter << mPosResult;
    }

    std::cout << "Testing Negative Samples" << std::endl;
    myImageSequence oNegWriter(sSubFolder + "Negative/", "", "jpg");
    oNegWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    while (oNegReader >> mNegImg) {
        myFeatureExtractor oExtractor(mNegImg, BlockSize);
        oExtractor.EnableFeature(myFeatureExtractor::Features::HOG_WITH_L2_NORM);
        oExtractor.EnableFeature(myFeatureExtractor::Features::LBP_8_1_UNIFORM);

        cv::Mat mNegResult = cv::Mat::ones(cv::Size2i(ImgSize.width * 2, ImgSize.height), CV_8UC1) * 127;
        for (int y = BlockSize.height, iPos = 0; y < mNegImg.rows - BlockSize.height; y += BlockSize.height) {
            for (int x = BlockSize.width; x < mNegImg.cols - BlockSize.width; x += BlockSize.width, ++iPos) {
                std::vector<float> vfFeature;
                cv::Point2i Position(x, y);
                oExtractor.Describe(Position, vfFeature);
                auto iIndex = oIndexr.GetBinNumber(mNegImg, Position);
                auto fResult = voCollector.at(iPos).Predict(iIndex, vfFeature);
                unsigned char uc = (fResult == +1) ? 255u : (fResult == -1) ? 0u : 127u;
                mNegResult(cv::Rect2i(cv::Point2i(64 + x, y), BlockSize)) = cv::Scalar::all(uc);
            }
        }
        mNegImg.copyTo(mNegResult(cv::Rect2i(cv::Point2i(0, 0), cv::Size2i(64, 128))));
        oNegWriter << mNegResult;
    }
    return 0;
}