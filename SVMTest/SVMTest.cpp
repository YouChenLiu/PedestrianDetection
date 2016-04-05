#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/myFeatureExtractor/myFeatureExtractor.h"
#include "../myLibrary/myClassifier/mySVM/mySVM.h"
#include <opencv2/highgui.hpp>
#include <fstream>

int main(void) {
    const cv::Size2i BlockSize(8, 8);
    const std::string sRootPath = "D:/Database/02/Night/";
    const std::vector<int> viFeatureSet = {
        myFeatureExtractor::Features::HOG_WITH_L2_NORM,
        myFeatureExtractor::Features::LBP_8_1_UNIFORM
    };

    struct Score {
        unsigned int TruePositive = 0;
        unsigned int TrueNegative = 0;
        unsigned int FalsePositive = 0;
        unsigned int FalseNegative = 0;
    } score;

    //  read the smv model
    mySVM oSVM("../SVMTraining/Model.xml");
    std::ofstream FileList("Dense.txt");
    
    {
        // read the positive smaples and calculate the hog feature
        myImageSequence oPositiveReader(sRootPath + "Positive/", "", "bmp", false);
        oPositiveReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
        std::cout << "loading positive images" << std::endl;
        cv::Mat mPositiveSample;
        while (oPositiveReader >> mPositiveSample) {
            std::cout << "\r" << oPositiveReader.GetSequenceNumberString();
            myFeatureExtractor oExtractor(mPositiveSample, BlockSize);
            for (const auto i : viFeatureSet) {
                oExtractor.EnableFeature(i);
            }
            std::vector<std::vector<float>> vvfHOGFeature;

            for (int y = 0; y < mPositiveSample.rows / BlockSize.height; ++y) {
                for (int x = 0; x < mPositiveSample.cols / BlockSize.width; ++x) {
                    std::vector<float> vfFeature;
                    oExtractor.Describe(cv::Point2i(x * BlockSize.width, y * BlockSize.height), vfFeature);
                    vvfHOGFeature.push_back(vfFeature);
                }
            }

            cv::Mat mSample(1, static_cast<int>(vvfHOGFeature.size() * vvfHOGFeature.at(0).size()), CV_32FC1);
            int i = 0;
            for (const auto& vfHOGFeature : vvfHOGFeature) {
                for (const auto fFeature : vfHOGFeature) {
                    mSample.at<float>(0, i++) = fFeature;
                }
            }
            
            std::string sResult = "\n";
            auto result = oSVM.Predict(mSample);
            if (result != 1) {
                ++score.FalseNegative;
                std::string sPath = "Wrong/" + std::string("pos") + oPositiveReader.GetSequenceNumberString() + std::string(".jpg");
                sResult = "pos" + oPositiveReader.GetSequenceNumberString() + sResult;
                cv::imwrite(sPath, mPositiveSample);
            } else {
                ++score.TruePositive;
            }
            FileList << sResult;
        }
    }
    {
        //read the negative smaples and calculate the hog feature
        myImageSequence oNegativeReader(sRootPath + "Negative/", "", "bmp", false);
        oNegativeReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
        std::cout << std::endl << "loading negative images" << std::endl;
        cv::Mat mNegativeSample;
        std::vector<std::vector<float>> vvfNegativeFeatures;
        while (oNegativeReader >> mNegativeSample) {
            std::cout << "\r" << oNegativeReader.GetSequenceNumberString();
            myFeatureExtractor oExtractor(mNegativeSample, BlockSize);
            for (const auto i : viFeatureSet) {
                oExtractor.EnableFeature(i);
            }

            std::vector<std::vector<float>> vvfHOGFeature;
            for (int y = 0; y < mNegativeSample.rows / BlockSize.height; ++y) {
                for (int x = 0; x < mNegativeSample.cols / BlockSize.width; ++x) {
                    std::vector<float> vfFeature;
                    oExtractor.Describe(cv::Point2i(x * BlockSize.width, y * BlockSize.height), vfFeature);
                    vvfHOGFeature.push_back(vfFeature);
                }
            }

            cv::Mat mSample(1, static_cast<int>(vvfHOGFeature.size() * vvfHOGFeature.at(0).size()), CV_32FC1);
            int i = 0;
            for (const auto& vfHOGFeature : vvfHOGFeature) {
                for (const auto fFeature : vfHOGFeature) {
                    mSample.at<float>(0, i++) = fFeature;
                }
            }
            
            std::string sResult = "\n";
            auto result = static_cast<int>(oSVM.Predict(mSample));
            if (result != -1) {
                ++score.FalsePositive;
                std::string sPath = "Wrong/" + std::string("neg") + oNegativeReader.GetSequenceNumberString() + std::string(".jpg");
                sResult = "neg" + oNegativeReader.GetSequenceNumberString() + sResult;
                cv::imwrite(sPath, mNegativeSample);
            } else {
                ++score.TrueNegative;
            }
            FileList << sResult;
        }
    }

    std::cout << std::endl
        << "TP: " << score.TruePositive << std::endl
        << "FP: " << score.FalsePositive << std::endl
        << "TN: " << score.TrueNegative << std::endl
        << "FN: " << score.FalseNegative << std::endl;
    system("pause");
    return 0;
}