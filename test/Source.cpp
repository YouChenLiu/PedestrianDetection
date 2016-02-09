#include "../myLibrary/myClassifier/mySVM/mySVM.h"
#include "../myLibrary/myModelCollector/myModelCollector.h"
#include "../myLibrary/myModelIndexer/myLBPIndexer/myLBPIndexer.h"
#include <memory>

int main(void) {
    /*
    mySVM oClassifier;
    oClassifier.AddSample(+1, std::vector<float>(10, +1.0f));
    oClassifier.AddSample(-1, std::vector<float>(10, -1.0f));

    oClassifier.SaveFeatures("feature.txt");
    */

    cv::Mat mImage = cv::Mat::zeros(64, 64, CV_8UC1);
    auto poIndexer = std::make_unique<myModelIndexerBase>(new myLBPIndexer(mImage));
    myModelCollector oController(mImage, std::move(poIndexer));
    auto poExtractor = oController.GetExtractor();
    poExtractor->SetImage(mImage);
    return 0;
}