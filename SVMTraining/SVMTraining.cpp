#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>
#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/myFeatureExtractor/myFeatureExtractor.h"

int main(void) {
    const cv::Size2i BlockSize(8, 8);
    const std::string sRootPath = "C:/Users/youch/Desktop/thermal_training_data/";
    
    // read the positive smaples and calculate the hog feature
    myImageSequence oPositiveReader(sRootPath + "Positive/", "", "bmp", false);
    oPositiveReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    std::cout << "loading positive images" << std::endl;
    cv::Mat mPositiveSample;
    std::vector<std::vector<float>> vvfPositiveFeatures;
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

        vvfPositiveFeatures.push_back(vfCascadeFeature);
    }

    std::cout << "Total Positive Sample : " << vvfPositiveFeatures.size() << std::endl;

    // read the negative smaples and calculate the hog feature

    myImageSequence oNegativeReader(sRootPath + "Negative/", "", "bmp", false);
    oNegativeReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    std::cout << "loading negative images" << std::endl;
    cv::Mat mNegativeSample;
    std::vector<std::vector<float>> vvfNegativeFeatures;
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

        vvfNegativeFeatures.push_back(vfCascadeFeature);
    }

    std::cout << "Total Negative Sample : " << vvfNegativeFeatures.size() << std::endl;

    // create matrix for saving labels for positive (+1) and negative (-1)
    int iNumberOfSamples = static_cast<int>(vvfPositiveFeatures.size()) + static_cast<int>(vvfNegativeFeatures.size());
#   if CV_MAJOR_VERSION < 3
        cv::Mat mLabels(iNumberOfSamples, 1, CV_32FC1);
        for (int y = 0; y < static_cast<int>(vvfPositiveFeatures.size()); ++y) {
            mLabels.at<float>(y, 0) = 1.0f;
        }
        for (int y = static_cast<int>(vvfPositiveFeatures.size()); y < iNumberOfSamples; ++y) {
            mLabels.at<float>(y, 0) = -1.0f;
        }
#   else
        cv::Mat mLabels(iNumberOfSamples, 1, CV_32SC1);
        for (int y = 0; y < static_cast<int>(vvfPositiveFeatures.size()); ++y) {
            mLabels.at<int>(y, 0) = 1;
        }
        for (int y = static_cast<int>(vvfPositiveFeatures.size()); y < iNumberOfSamples; ++y) {
            mLabels.at<int>(y, 0) = -1;
        }
#   endif
    
    int iNumberOfFeatures = static_cast<int>(vvfPositiveFeatures.at(0).size());
    cv::Mat mFeatures(iNumberOfSamples, iNumberOfFeatures, CV_32FC1);
    for (int y = 0; y < static_cast<int>(vvfPositiveFeatures.size()); ++y) {
        for (int x = 0; x < iNumberOfFeatures; ++x) {
            mFeatures.at<float>(y, x) = vvfPositiveFeatures.at(y).at(x);
        }
    }
    int row = 0;
    for (int y = static_cast<int>(vvfPositiveFeatures.size()); y < iNumberOfSamples; ++y) {
        for (int x = 0; x < iNumberOfFeatures; ++x) {
            mFeatures.at<float>(y, x) = vvfNegativeFeatures.at(row).at(x);
        }
        ++row;
    }

    //  clean the feature vector for reducing the memory
    for (auto& v : vvfPositiveFeatures) {
        v.clear();
    }
    vvfPositiveFeatures.clear();

    for (auto& v : vvfNegativeFeatures) {
        v.clear();
    }
    vvfNegativeFeatures.clear();

    // create svm for training and then save the result to a xml file.
#   if CV_MAJOR_VERSION < 3
        cv::CvSVMParams oSVMParameters;
        oSVMParameters.svm_type = SVM::C_SVC;
        oSVMParameters.C = 0.01;
        oSVMParameters.kernel_type = SVM::LINEAR;
        oSVMParameters.term_crit = TermCriteria(CV_TERMCRIT_ITER, (int)100000, 1e-6);

        cv::SVM oSVM;
        oSVM.train(mFeatures, mLabels, Mat(), Mat(), oSVMParameters);
        oSVM.save("Result.xml");
#   else
        cv::Ptr<cv::ml::SVM> poSVM = cv::ml::SVM::create();
        poSVM->setType(cv::ml::SVM::C_SVC);
        poSVM->setC(0.6);
        poSVM->setKernel(cv::ml::SVM::LINEAR);
        poSVM->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 10000000, 1e-6));
        std::cout << "training..." << std::endl;
        poSVM->train(mFeatures, cv::ml::ROW_SAMPLE, mLabels);
        poSVM->save("Model.xml");
#   endif

    return 0;
}
