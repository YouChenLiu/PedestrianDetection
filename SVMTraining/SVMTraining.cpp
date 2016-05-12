#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/myFeatureDescriptor/myBlockDescriptor/myBlockDescriptor.h"
#include "../myLibrary/myClassifier/mySVM/mySVM.h"

int main(void) {
    const cv::Size2i BlockSize(8, 8);
    const std::string sRootPath = "D:/Database/01/All/";
    const std::vector<int> viFeatureSet = { 
        Descriptor::myBlockDescriptor::Feature::HOG_STANDARD | Descriptor::myBlockDescriptor::Feature::L2_NORM,
        Descriptor::myBlockDescriptor::Feature::LBP_8_1_UNIFORM
    };
    
    // create a svm for training
    Classifier::mySVM oSVM;
    {
        // read the positive smaples and calculate the hog feature
        myImageSequence oPositiveReader(sRootPath + "Positive/", "", "bmp", false);
        std::cout << "loading positive images" << std::endl;
        cv::Mat mPositiveSample;
        while (oPositiveReader >> mPositiveSample) {
            const std::string sSeqNumber = oPositiveReader.GetSequenceNumberString();
            std::cout << "\r" << sSeqNumber;

            Descriptor::myBlockDescriptor oExtractor(mPositiveSample, BlockSize);
            for (const auto i : viFeatureSet) {
                oExtractor.EnableFeature(i);
            }

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
    }
    {
        // read the negative smaples and calculate the hog feature
        myImageSequence oNegativeReader(sRootPath + "Negative/", "", "bmp", false);
        std::cout << std::endl << "loading negative images" << std::endl;
        cv::Mat mNegativeSample;
        while (oNegativeReader >> mNegativeSample) {
            const std::string sSeqNumber = oNegativeReader.GetSequenceNumberString();
            std::cout << "\r" << sSeqNumber;

            Descriptor::myBlockDescriptor oExtractor(mNegativeSample, BlockSize);
            for (const auto i : viFeatureSet) {
                oExtractor.EnableFeature(i);
            }

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
    }

    std::cout << std::endl << "Training Start" << std::endl;
    oSVM.TrainAuto(4);
    oSVM.Save("Model.xml");
    std::cout << "Training Finish" << std::endl;

    return 0;
}
