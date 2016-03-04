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
    // path for sample images
    const std::string sRootPath = "D:/Backup/Thermal/noon/";

    // folder name for write out images
    const std::string sSubFolder = "Morning+Night-Noon/";

    // vector of collectors
    std::vector<myModelCollector> voCollector(iCollectorCount);
    for (auto& o : voCollector) {
        o.Resize(59);
    }

    myLBPIndexer oIndexr(BlockSize);

    // reading positive image sequence
    myImageSequence oPosReader(sRootPath + "Positive/", "", "bmp", false);
    oPosReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);

    // reading negative images
    myImageSequence oNegReader(sRootPath + "Negative/", "", "bmp", false);
    oNegReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    
    cv::Mat mPosImg;
    cv::Mat mNegImg;
    /*
    std::cout << "Reading Positive Samples" << std::endl;
    while (oPosReader >> mPosImg) {
        std::cout << "Reading " << oPosReader.GetSequenceNumberString() << "\r";
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
    std::cout << std::endl << oPosReader.GetSequenceNumber()<< " Positive Samples" << std::endl;

    std::cout << "Reading Negative Samples" << std::endl;
    while (oNegReader >> mNegImg) {
        std::cout << "Reading " << oNegReader.GetSequenceNumberString() << "\r";
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
    std::cout <<std::endl << oNegReader.GetSequenceNumber()<< " Negative Samples" << std::endl;

    std::ofstream FeatureList("feature.txt");
    FeatureList << voCollector.size() << std::endl;
    for (std::size_t i = 0; i < voCollector.size(); ++i) {
        std::cout << "Save features in collector : " << i << " / " << iCollectorCount - 1 << "\r";
        FeatureList << voCollector.at(i).SaveFeatures("Features") << std::endl;
    }
    std::cout << std::endl << "All features are saved" << std::endl;
    
    std::ofstream ModelList("models.txt");
    ModelList << voCollector.size() << std::endl;
    for (std::size_t i = 0; i < voCollector.size(); ++i) {
        std::cout << "Training model collector : " << i << " / " << iCollectorCount - 1 << "\r";
        voCollector.at(i).TrainModels();
        ModelList << voCollector.at(i).SaveModels("Models") << std::endl;
    }
    std::cout << std::endl << "All models are trained and saved" << std::endl;
    */
    
    // testing
    
    // reading models
    std::cout << "Reading saved models" << std::endl;
    std::ifstream ModelList("models.txt");
    int iModelsCount = 0;
    ModelList >> iModelsCount;
    for (std::size_t i = 0; i < iModelsCount; ++i) {
        std::string sPath;
        ModelList >> sPath;
        voCollector.at(i).LoadModels(sPath);
        std::cout << "Reading models : " << i << " / " << iModelsCount - 1 << "\r";
    }

    std::cout << std::endl << "Testing Positive Samples" << std::endl;
    std::string sInstruction = "mkdir \"" + sSubFolder + "Positive\"";
    system(sInstruction.c_str());
    myImageSequence oPosWriter(sSubFolder + "Positive/", "", "jpg");
    oPosWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    while (oPosReader >> mPosImg) {
        std::cout << "Reading " << oPosReader.GetSequenceNumberString() << "\r";
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

    std::cout << std::endl << "Testing Negative Samples" << std::endl;
    sInstruction = "mkdir \"" + sSubFolder + "Negative\"";
    system(sInstruction.c_str());
    myImageSequence oNegWriter(sSubFolder + "Negative/", "", "jpg");
    oNegWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    while (oNegReader >> mNegImg) {
        std::cout << "Reading " << oNegReader.GetSequenceNumberString() << "\r";
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