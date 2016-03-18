#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/myFeatureExtractor/myFeatureExtractor.h"
#include "../myLibrary/myClassifier/mySVM/mySVM.h"
#include <opencv2/highgui.hpp>

int main(void) {
    using namespace std;

    const cv::Size2i BlockSize(8, 8);
    const string sRootPath = "D:/Database/02/";
    const std::vector<int> viFeatureSet = {
        myFeatureExtractor::Features::HOG_WITH_L2_NORM,
        myFeatureExtractor::Features::LBP_8_1_UNIFORM
    };

    //  read the smv model
    mySVM oSVM("../SVMTraining/Model.xml");
    
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
            vector<vector<float>> vvfHOGFeature;

            for (int y = 0; y < mPositiveSample.rows / BlockSize.height; ++y) {
                for (int x = 0; x < mPositiveSample.cols / BlockSize.width; ++x) {
                    vector<float> vfFeature;
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

            auto result = static_cast<int>(oSVM.Predict(mSample));
            if (result != 1) {
                std::string sPath = "Wrong/" + std::string("pos") + oPositiveReader.GetSequenceNumberString() + std::string(".jpg");
                cv::imwrite(sPath, mPositiveSample);
            }
        }
    }
    {
        //read the negative smaples and calculate the hog feature
        myImageSequence oNegativeReader(sRootPath + "Negative/", "", "bmp", false);
        oNegativeReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
        std::cout << std::endl << "loading negative images" << std::endl;
        cv::Mat mNegativeSample;
        vector<vector<float>> vvfNegativeFeatures;
        while (oNegativeReader >> mNegativeSample) {
            std::cout << "\r" << oNegativeReader.GetSequenceNumberString();
            myFeatureExtractor oExtractor(mNegativeSample, BlockSize);
            for (const auto i : viFeatureSet) {
                oExtractor.EnableFeature(i);
            }

            vector<vector<float>> vvfHOGFeature;
            for (int y = 0; y < mNegativeSample.rows / BlockSize.height; ++y) {
                for (int x = 0; x < mNegativeSample.cols / BlockSize.width; ++x) {
                    vector<float> vfFeature;
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

            auto result = static_cast<int>(oSVM.Predict(mSample));
            if (result != -1) {
                std::string sPath = "Wrong/" + std::string("neg") + oNegativeReader.GetSequenceNumberString() + std::string(".jpg");
                cv::imwrite(sPath, mNegativeSample);
            }
        }
    }
    return 0;
}