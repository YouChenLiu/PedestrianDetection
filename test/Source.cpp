#include "../myLibrary/myClassifier/mySVM/mySVM.h"
#include "../myLibrary/myModelCollector/myModelCollector.h"
#include "../myLibrary/myFeatureExtractor/myFeatureExtractor.h"
#include "../myLibrary/myModelIndexer/myLBPIndexer/myLBPIndexer.h"
#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/myHistoAnalyzer/myHistoAnalyzer.h"
#include <iomanip>

int main(void) {
    const cv::Size2i ImgSize(64, 128);
    const cv::Size2i BlockSize(8, 16);
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
    // training models
    /*
    // load positive samples
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

    // load negative samples
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

    // write out features
    std::ofstream FeatureList("feature.txt");
    FeatureList << voCollector.size() << std::endl;
    for (std::size_t i = 0; i < voCollector.size(); ++i) {
        FeatureList << voCollector.at(i).SaveFeatures("Features") << std::endl;
    }
    
    // write out models
    std::ofstream ModelList("models.txt");
    ModelList << voCollector.size() << std::endl;
    for (std::size_t i = 0; i < voCollector.size(); ++i) {
        voCollector.at(i).TrainModels();
        ModelList << voCollector.at(i).SaveModels("Models") << std::endl;
    }
    */

    // testing models
    /*
    // reading models
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
    */

    // check Hausdroff Distance

    std::array<std::string, 3> asTime = { "morning", "noon", "night" };
    const std::string s1 = "Positive";
    const std::string s2 = "Positive";

    for (size_t i = 0; i < asTime.size(); ++i) {
        for (size_t j = 0; j < asTime.size(); ++j) {
            std::vector<myHistoAnalyer> vHisto1;
            myImageSequence oReader1("D:/Backup/Thermal/" + asTime.at(i) + "/" + s1 + "/", "", "bmp", false);
            oReader1.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
            cv::Mat mImg1;
            while (oReader1 >> mImg1) {
                std::vector<float> vfHisto(59, 0.0f);
                for (int y = BlockSize.height; y < mImg1.rows - BlockSize.height; y += BlockSize.height) {
                    for (int x = BlockSize.width; x < mImg1.cols - BlockSize.width; x += BlockSize.width) {
                        cv::Point2i Position(x, y);
                        ++vfHisto.at(oIndexr.GetBinNumber(mImg1, Position));
                    }
                }
                vHisto1.push_back(myHistoAnalyer(vfHisto, false));
            }
            std::cout << oReader1.GetSequenceNumber() << " Images" << std::endl;

            std::vector<myHistoAnalyer> vHisto2;
            myImageSequence oReader2("D:/Backup/Thermal/" + asTime.at(j) + "/" + s2 + "/", "", "bmp", false);
            oReader2.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
            cv::Mat mImg2;
            while (oReader2 >> mImg2) {
                std::vector<float> vfHisto(59, 0.0f);
                for (int y = BlockSize.height; y < mImg2.rows - BlockSize.height; y += BlockSize.height) {
                    for (int x = BlockSize.width; x < mImg2.cols - BlockSize.width; x += BlockSize.width) {
                        cv::Point2i Position(x, y);
                        ++vfHisto.at(oIndexr.GetBinNumber(mImg2, Position));
                    }
                }
                vHisto2.push_back(myHistoAnalyer(vfHisto, false));
            }
            std::cout << oReader2.GetSequenceNumber() << " Images" << std::endl;

            float fHausdroff = 0.0f;
            for (const auto& b1 : vHisto1) {
                float fMinDistance = 9999.0f;
                for (const auto& b2 : vHisto2) {
                    auto fDistance = b1.CalculateDistance(b2, myHistoAnalyer::Distance::OVERLAP);
                    if (fMinDistance > fDistance) {
                        fMinDistance = fDistance;
                    }
                }
                if (fHausdroff < fMinDistance) {
                    fHausdroff = fMinDistance;
                }
            }
            std::cout << asTime.at(i) << ":" << s1 << " -> " << asTime.at(j) << ":" << s2 << "\t";
            std::cout << fHausdroff << std::endl;
        }
    }
 
    return 0;
}