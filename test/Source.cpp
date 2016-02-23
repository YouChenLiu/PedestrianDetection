#include "../myLibrary/myClassifier/mySVM/mySVM.h"
#include "../myLibrary/myModelCollector/myModelCollector.h"
#include "../myLibrary/myFeatureExtractor/myFeatureExtractor.h"
#include "../myLibrary/myModelIndexer/myLBPIndexer/myLBPIndexer.h"
#include "../myLibrary/myImageSequence/myImageSequence.h"
#include <memory>
#include <opencv2/highgui.hpp>
#include "../myLibrary/myClassifier/mySVM/mySVM.h"
#include <opencv2/ml.hpp>

int main(void) {
    /*
    cv::Size2i BlockSize(8, 8);
    cv::Mat mImage = cv::imread("000001.bmp", cv::IMREAD_GRAYSCALE);
    auto poExtractor = std::make_unique<myFeatureExtractor>(mImage, cv::Size2i(8, 8));
    poExtractor->EnableFeature(myFeatureExtractor::Features::HOG_WITH_L2_NORM);
    poExtractor->EnableFeature(myFeatureExtractor::Features::LBP_8_1_UNIFORM);
    auto poCollector = std::make_unique<myModelCollector>(59);
    auto poIndexer = std::make_unique<myLBPIndexer>();
    cv::Point2i pt(8, 8);
    std::vector<float> vfFeature;
    auto iBinNumber = poIndexer->GetBinNumber(mImage, pt);
    poExtractor->Describe(pt, vfFeature);
    poCollector->AddSample(iBinNumber, 1, vfFeature);
    */

    std::array<myModelCollector, 5> aoCollector;

    const std::string sRoot = "Models";
    int i = 0;
    for (auto& o : aoCollector) {
        o.Resize(5);
        o.AddSample(i, +1, std::vector<float>(10, +1.0f));
        o.AddSample(i, -1, std::vector<float>(10, -1.0f));
        o.TrainModels();
        o.SaveModels(sRoot);
        i++;
    }
    
    return 0;
}