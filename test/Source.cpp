#include "../myLibrary/myClassifier/mySVM/mySVM.h"
#include "../myLibrary/myClassifier/myAdaBoost/myAdaBoost.h"
#include "../myLibrary/myModelCollector/myModelCollector.h"
#include "../myLibrary/myFeatureExtractor/myFeatureExtractor.h"
#include "../myLibrary/myModelIndexer/myLBPIndexer/myLBPIndexer.h"
#include <fstream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(void) {
    const cv::Size2i ImgSize(64, 128);
    const cv::Size2i BlockSize(8, 8);
    const int iCollectorCount = (((ImgSize.height - 2 * BlockSize.height) / 8) *
                                 ((ImgSize.width - 2 * BlockSize.width) / 8));
    myLBPIndexer oIndexr(BlockSize);
    myFeatureExtractor oExtractor(cv::Mat(), BlockSize);
    const std::vector<int> viFeature = {
        myFeatureExtractor::Features::HOG_WITH_L2_NORM,
        myFeatureExtractor::Features::LBP_8_1_UNIFORM
    };
    for (auto feature : viFeature) {
        oExtractor.EnableFeature(feature);
    }
    std::cout << "Reading saved models" << std::endl;
    std::ifstream ModelList("../TwoLayersDetection/LIN_Models.txt");
    // vector of collectors
    std::vector<myModelCollector> voCollector(84);
    for (auto& o : voCollector) {
        o.Resize(59);
    }
    int iModelsCount = 0;
    ModelList >> iModelsCount;
    for (std::size_t i = 0; i < iModelsCount; ++i) {
        std::string sPath;
        ModelList >> sPath;
        voCollector.at(i).LoadModels("../TwoLayersDetection/" + sPath);
        std::cout << "Reading models : " << i << " / " << iModelsCount - 1 << "\r";
    }
    std::cout << std::endl;
    mySupervisedClassifier* oL2Classifier = new myAdaBoost(70);
    oL2Classifier->Load("../TwoLayersDetection/A_L2_70.xml");

    const std::string sRoot("D:/Database/02/Night/");
    std::ifstream List("Dense.txt");
    std::string str;

    while (std::getline(List, str)) {
        if (str.length() == 0) {
            continue;
        }

        std::string sFileName;
        std::string sPN = str.substr(0, 3);
        if (str.substr(0, 3).compare("pos") == 0) {
            sFileName = "Positive/" + str.substr(3, 6) + ".bmp";
        } else {
            sFileName = "Negative/" + str.substr(3, 6) + ".bmp";
        }

        std::string sPath = sRoot + sFileName;
        cv::Mat mImg = cv::imread(sPath, cv::IMREAD_GRAYSCALE);
        
        cv::Mat mDistribution = cv::Mat::ones(ImgSize, CV_8UC1) * 127;
        oExtractor.SetImage(mImg);
        std::vector<float> vfResult(iCollectorCount, 0.0f);
        for (int y = BlockSize.height, iPos = 0; y < mImg.rows - BlockSize.height; y += BlockSize.height) {
            for (int x = BlockSize.width; x < mImg.cols - BlockSize.width; x += BlockSize.width, ++iPos) {
                std::vector<float> vfFeature;
                cv::Point2i Position(x, y);
                oExtractor.Describe(Position, vfFeature);
                auto iIndex = oIndexr.GetBinNumber(mImg, Position);
                auto fResult = voCollector.at(iPos).Predict(iIndex, vfFeature);
                vfResult.at(iPos) = fResult;
                auto Color = (fResult == -1) ? cv::Scalar::all(0) :
                    ((fResult == 1) ? cv::Scalar::all(255) :
                     cv::Scalar::all(127));
                
                auto Region = cv::Rect2i(Position, BlockSize);
                mDistribution(Region).setTo(Color);
                
            }
        }
        
        cv::Mat mResult = cv::Mat::zeros(cv::Size2i(128, 128), CV_8UC1);
        mImg.copyTo(mResult(cv::Rect2i(0, 0, 64, 128)));
        mDistribution.copyTo(mResult(cv::Rect2i(64, 0, 64, 128)));
        auto DetectingResult = oL2Classifier->Predict(vfResult);
        cv::putText(mResult,
                    (DetectingResult == -1) ? "-" : "+",
                    cv::Point2i(30, 60),
                    cv::FONT_HERSHEY_PLAIN,
                    1.0,
                    cv::Scalar::all(0));
        cv::imwrite("Image/" + str + ".jpg", mResult);
    }
    return 0;
}