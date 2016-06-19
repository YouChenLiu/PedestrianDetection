#include "myClassifier.h"
#include "myModelCollector/myModelCollector.h"
#include "myFeatureDescriptor/myBlockDescriptor/myBlockDescriptor.h"
#include "myModelIndexer/myLBPIndexer/myLBPIndexer.h"
#include "myImageSequence/myImageSequence.h"
#include "myPlugin/myScanner/myScanner.h"
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iomanip>

int main(void) {
  using std::string;
  using std::vector;

  // root path for training samples
  const string sTrainingSamplesRoot = "D:/Database/Sample/01/";
  // root path for testing samples
  const string sTestingSamplesRoot = "D:/Database/Sample/02/";
  const string sModelRoot = "../Models/";
  // determind do training or testing
  const bool bLoadingFeature = false;
  const bool bTrainingL1 = false;
  const bool bTrainingL2 = true;
  const bool bTesting = true;
  const bool bSaving = false;
  
  std::vector<cv::Rect2i> vRect;
  {
    Plugin::myScanner scanner(cv::Point2i(8, 8), cv::Point2i(56, 120));
    for (int h = 8; h <= 32; h += 8) {
      for (int w = 8; w <= 16; w += 8) {
        scanner.CalRect(vRect, cv::Size2i(w, h), cv::Point2i(8, 8));
      } // for w
    } // for h
  }

  const unsigned int uiWeakCount = 140;
  Classifier::mySupervisedClassifier* oL2Classifier = new Classifier::myAdaBoost(uiWeakCount);
  string sL2Model = "A_L2_GENERAL_";
  {
    std::stringstream ss;
    ss << uiWeakCount;
    string sWeakCount = ss.str();
    sL2Model += sWeakCount + ".xml";
  }
  const string sModelName = "GENERAL_TEST_Model_1";

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
  const std::map<string, int> Labels = { {"Positive", +1}, {"Negative", -1} };
  // vector for feature set
  const vector<int> viFeature = {
      Descriptor::myBlockDescriptor::Feature::HOG_SINGLE_CELL | Descriptor::myBlockDescriptor::Feature::L2_NORM,
      Descriptor::myBlockDescriptor::Feature::LBP_8_1_UNIFORM// | Descriptor::myBlockDescriptor::Feature::L1_NORM
  };
  
  // feature extractor
  Descriptor::myBlockDescriptor oExtractor(cv::Mat(), cv::Size2i(8, 8));
  for (auto feature : viFeature) {
    oExtractor.EnableFeature(feature);
  }

  struct Score {
    unsigned int TruePositive = 0;
    unsigned int TrueNegative = 0;
    unsigned int FalsePositive = 0;
    unsigned int FalseNegative = 0;
  } score;

  std::cout << sModelName << std::endl;

  if (bTrainingL1) {
    std::cout << "Start Training" << std::endl;
    std::cout << "Training Layer 1 Classifier" << std::endl;
    if (bLoadingFeature) {
      std::cout << "load feature from text file" << std::endl;
      const string sFeaturePath = sModelName + "_features.txt";
      std::ifstream FeatureFileList(sFeaturePath);
      string sCount;
      FeatureFileList >> sCount;
      // get total count
      std::stringstream ssCount(sCount);
      unsigned int iCount = 0;
      ssCount >> iCount;
      for (size_t i = 0; i < iCount; ++i) {
        string sFeature;
        FeatureFileList >> sFeature;
        std::cout << "\rLoading " << i << " / " << iCount - 1;
        voCollector.at(i).LoadFeatures(sFeature);
      }
      std::cout << std::endl;
    } else {
      for (auto sTime : vsTimeInterval) {
        for (const auto& sPN : vsPosNeg) {
          string sSamplePath = sTrainingSamplesRoot + sTime + "/" + sPN + "/";
          myImageSequence oReader(sSamplePath, "", "bmp", false);
          cv::Mat mImg;
          while (oReader >> mImg) {
            std::cout << "\rReading " + sTime + "-" + sPN + ":" + oReader.GetSequenceNumberString();
            oExtractor.SetImage(mImg);

            for (size_t iPos = 0; iPos < vRect.size(); ++iPos) {
              vector<float> vfFeature;
              vfFeature.reserve(68);
              oExtractor.Describe(vRect.at(iPos), vfFeature);
              auto iIndex = oIndexr.GetBinNumber(mImg, vRect.at(iPos));
              voCollector.at(iPos).AddSample(iIndex, Labels.at(sPN), vfFeature);
            } //for
          } // while
          std::cout << std::endl;
        } // for
      } // for
    } // else

    // train and save all layer 1 models
    std::ofstream ModelList(sModelRoot + sModelName + ".txt");
    ModelList << voCollector.size() << std::endl;
    for (std::size_t i = 0; i < voCollector.size(); ++i) {
      std::cout << "Training model collector : " << i << " / " << vRect.size() - 1 << std::endl;
      voCollector.at(i).TrainModels();
      ModelList << voCollector.at(i).SaveModels(sModelName, sModelRoot) << std::endl;
      std::cout << std::endl;
      //voCollector.at(i).Clear();
    }
  } else {
    std::cout << "Reading saved layer 1 models" << std::endl;
    std::ifstream ModelList(sModelRoot + sModelName + ".txt");
    int iModelsCount = 0;
    ModelList >> iModelsCount;
    for (std::size_t i = 0; i < iModelsCount; ++i) {
      std::string sPath;
      ModelList >> sPath;
      voCollector.at(i).LoadModels(sModelRoot + sPath);
      std::cout << "\rReading models : " << i << " / " << iModelsCount - 1;
    }
    std::cout << std::endl;
  }

  if (bTrainingL2) {
    std::cout << std::endl << "Training Layer 2 Classifier" << std::endl;
    for (auto sTime : vsTimeInterval) {
      for (const auto& sPN : vsPosNeg) {
        std::string sSamplePath = sTrainingSamplesRoot + sTime + "/" + sPN + "/";
        myImageSequence oReader(sSamplePath, "", "bmp", false);
        cv::Mat mImg;
        while (oReader >> mImg) {
          std::cout << "\rReading " + sTime + "-" + sPN + ":" + oReader.GetSequenceNumberString();
          oExtractor.SetImage(mImg);
          vector<float> vfResult(vRect.size(), 0.0f);
          for (size_t iPos = 0; iPos < vRect.size(); ++iPos) {
            vector<float> vfFeature;
            vfFeature.reserve(68);
            oExtractor.Describe(vRect.at(iPos), vfFeature);
            auto iIndex = oIndexr.GetBinNumber(mImg, vRect.at(iPos));
            auto fResult = voCollector.at(iPos).Predict(iIndex, vfFeature);
            if (fResult == NAN) {
              fResult = -1.0f;
            }
            vfResult.at(iPos) = fResult;
          }
          oL2Classifier->AddSample(Labels.at(sPN), vfResult);
          
        }
        std::cout << std::endl;
      }
    }

    std::cout << "Find " << uiWeakCount << " weak classifiers" << std::endl;
    oL2Classifier->Train();
    oL2Classifier->Save(sModelRoot + sL2Model);
    std::cout << "Training Finish" << std::endl;
  } else {
    std::cout << "Load L2 Classifier" << std::endl;
    oL2Classifier->Load(sModelRoot + sL2Model);
  }

  if (bTesting) {
    system("mkdir \"Wrong\"");
    std::ofstream ListFile(sModelName + "_WRONG.txt");
    for (auto sTime : vsTimeInterval) {
      for (const auto& sPN : vsPosNeg) {
        string sSamplePath = sTestingSamplesRoot + sTime + "/" + sPN + "/";
        myImageSequence oReader(sSamplePath, "", "bmp", false);
        cv::Mat mImg;
        while (oReader >> mImg) {
          std::cout << "\rReading " + sTime + "-" + sPN + ":" + oReader.GetSequenceNumberString();
          oExtractor.SetImage(mImg);
          vector<float> vfResult(vRect.size(), 0.0f);
          for (size_t i = 0; i < vRect.size(); ++i) {
            vector<float> vfFeature;
            vfFeature.reserve(68);
            oExtractor.Describe(vRect.at(i), vfFeature);
            auto iIndex = oIndexr.GetBinNumber(mImg, vRect.at(i));
            auto fResult = voCollector.at(i).Predict(iIndex, vfFeature);
            vfResult.at(i) = fResult;
          }
          auto DetectingResult = oL2Classifier->Predict(vfResult);
          string sResult = "\n";
          if (DetectingResult >= 0 && Labels.at(sPN) >= 0) {        // true positive
            ++score.TruePositive;
          } else if (DetectingResult >= 0 && Labels.at(sPN) < 0) {  // false positive
            ++score.FalsePositive;
          } else if (DetectingResult < 0 && Labels.at(sPN) >= 0) {  // false negative
            ++score.FalseNegative;
          } else {                                                  // true negative
            ++score.TrueNegative;
          }
        }
        std::cout << std::endl;
      }
    }
  }

  std::cout << "TP: " << score.TruePositive << std::endl
    << "FP: " << score.FalsePositive << std::endl
    << "TN: " << score.TrueNegative << std::endl
    << "FN: " << score.FalseNegative << std::endl;

  return 0;
}