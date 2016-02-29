#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/myFeatureExtractor/myFeatureExtractor.h"
#include "../myLibrary/myClassifier/mySVM/mySVM.h"

int main(void) {
    const cv::Size2i BlockSize(8, 8);
    const std::string sRootPath = "D:/Backup/Thermal/morning/";

    // create a svm for training
    mySVM oSVM;
    
    // read the positive smaples and calculate the hog feature
    myImageSequence oPositiveReader(sRootPath + "Positive/", "", "bmp", false);
    oPositiveReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    std::cout << "loading positive images" << std::endl;
    cv::Mat mPositiveSample;
    while (oPositiveReader >> mPositiveSample) {
        myFeatureExtractor oExtractor(mPositiveSample, BlockSize);
        oExtractor.EnableFeature(myFeatureExtractor::Features::HOG_WITHOUT_NORM);
        oExtractor.EnableFeature(myFeatureExtractor::Features::LBP_8_1_UNIFORM);
        std::vector<float> vfCascadeFeature;
        for (int y = 0; y < mPositiveSample.rows / BlockSize.height; ++y) {
            for (int x = 0; x < mPositiveSample.cols / BlockSize.width; ++x) {
                std::vector<float> vfFeature;
                oExtractor.Describe(cv::Point2i(x * BlockSize.width, y * BlockSize.height), vfFeature);
                for (auto f : vfFeature) {
                    vfCascadeFeature.push_back(f);
                }
            }
        }

        oSVM.AddSample(+1, vfCascadeFeature);
    }

    // read the negative smaples and calculate the hog feature

    myImageSequence oNegativeReader(sRootPath + "Negative/", "", "bmp", false);
    oNegativeReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    std::cout << "loading negative images" << std::endl;
    cv::Mat mNegativeSample;
    while (oNegativeReader >> mNegativeSample) {
        myFeatureExtractor oExtractor(mNegativeSample, BlockSize);
        oExtractor.EnableFeature(myFeatureExtractor::Features::HOG_WITHOUT_NORM);
        oExtractor.EnableFeature(myFeatureExtractor::Features::LBP_8_1_UNIFORM);

        std::vector<float> vfCascadeFeature;
        for (int y = 0; y < mNegativeSample.rows / BlockSize.height; ++y) {
            for (int x = 0; x < mNegativeSample.cols / BlockSize.width; ++x) {
                std::vector<float> vfFeature;
                oExtractor.Describe(cv::Point2i(x * BlockSize.width, y * BlockSize.height), vfFeature);
                for (auto f : vfFeature) {
                    vfCascadeFeature.push_back(f);
                }
            }
        }

        oSVM.AddSample(-1, vfCascadeFeature);
    }

    oSVM.Train();
    oSVM.Save("Model.xml");

    return 0;
}
