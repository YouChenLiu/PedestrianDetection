#include "../myLibrary/myPlugin.h"
#include "../myLibrary/myClassifier.h"
#include "../myLibrary/myModelCollector/myModelCollector.h"
#include "../myLibrary/myFeatureDescriptor/myBlockDescriptor/myBlockDescriptor.h"
#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/myModelIndexer/myLBPIndexer/mylbpindexer.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <iomanip>

int main(void) {
  using std::string;
  using std::vector;
  const int iNumModels = 549;
  for (size_t i = 104; i < iNumModels; ++i) {
    const string sRoot = "GENERAL_TEST_Model_1";
    myModelCollector oCollector(59);
    std::stringstream ss;
    ss << sRoot + "_feature" << "/" << std::setfill('0') << std::setw(4) << i << "/features.txt";
    const string sPath = ss.str();
    std::cout << "\rTraining " << i << " / " << iNumModels;
    oCollector.LoadFeatures(sPath);
    oCollector.TrainModels();
    oCollector.SaveModels(sRoot);
  }
  /*
  // root path for training samples
  const string sTrainingSamplesRoot = "D:/Database/01/";
  // root path for testing samples
  const string sTestingSamplesRoot = "D:/Database/02/";
  // determind do training or testing
  const bool bTrainingL1 = true;
  const bool bTrainingL2 = true;
  const bool bTesting = true;
  
  const cv::Size2i ImgSize(64, 128);
  const cv::Size2i BlockSize(8, 8);

  std::vector<cv::Rect2i> vRect;
  {
    Plugin::myScanner scanner(cv::Point2i(8, 8), cv::Point2i(56, 120));
    for (int h = 8; h <= 32; h += 8) {
      for (int w = 8; w <= 16; w += 8) {
        scanner.CalRect(vRect, cv::Size2i(w, h), cv::Point2i(8, 8));
      }
    }
  }

  Classifier::mySupervisedClassifier* oL2Classifier = new Classifier::myAdaBoost(150);
  const string sL2Model = "A_L2_GENERAL_150.xml";
  const string sModelName = "GENERAL_TEST_Model_1";

  const bool bSaving = false;
  myLBPIndexer oIndexr;
  // vector of collectors
  vector<myModelCollector> voCollector(vRect.size());
  for (auto& o : voCollector) {
    o.Resize(oIndexr.GetNumOfBins());
  }

  // define time intervals strings
  const vector<string> vsTimeInterval = { "Morning", "Noon", "Evening", "Night" };
  // array saves Pos and Neg string
  const std::array<string, 2> vsPosNeg = { "Positive", "Negative" };
  // array saves labels for pos and neg
  const std::array<int, 2> viAnswer = { +1, -1 };
  // vector for feature set
  const vector<int> viFeature = {
      Descriptor::myBlockDescriptor::Feature::HOG_SINGLE_CELL | Descriptor::myBlockDescriptor::Feature::L2_NORM,
      Descriptor::myBlockDescriptor::Feature::LBP_8_1_UNIFORM// | Descriptor::myBlockDescriptor::Feature::L1_NORM
  };
  
  // feature extractor
  Descriptor::myBlockDescriptor oExtractor(cv::Mat(), BlockSize);
  for (auto feature : viFeature) {
    oExtractor.EnableFeature(feature);
  }
  
  std::cout << "Start Training" << std::endl;
  std::cout << "Training Layer 1 Classifier" << std::endl;
  for (auto sTime : vsTimeInterval) {
    for (size_t i = 0; i < vsPosNeg.size(); ++i) {
      string sSamplePath = sTrainingSamplesRoot + sTime + "/" + vsPosNeg.at(i) + "/";
      myImageSequence oReader(sSamplePath, "", "bmp", false);
      cv::Mat mImg;
      while (oReader >> mImg) {
        std::cout << "\rReading " + sTime + "-" + vsPosNeg.at(i) + ":" + oReader.GetSequenceNumberString();
        oExtractor.SetImage(mImg);

        for (size_t iPos = 0; iPos < vRect.size(); ++iPos) {
          vector<float> vfFeature;
          vfFeature.reserve(68);
          oExtractor.Describe(vRect.at(iPos), vfFeature);
          auto iIndex = oIndexr.GetBinNumber(mImg, vRect.at(iPos));
          voCollector.at(iPos).AddSample(iIndex, viAnswer.at(i), vfFeature);
        }
      }
      std::cout << std::endl;
    }
  }

  // train and save all layer 1 models
  std::ofstream ModelList(sModelName + ".txt");
  ModelList << voCollector.size() << std::endl;
  for (std::size_t i = 0; i < voCollector.size(); ++i) {
    std::cout << "\rSaving model collector : " << i << " / " << vRect.size() - 1;
    voCollector.at(i).SaveFeatures(sModelName);
    ModelList << voCollector.at(i).SaveModels(sModelName) << std::endl;
  }
  */
  return 0;
}