#include "common.h"
#include "myClassifier/mySVM/mySVM.h"
#include "myFeatureDescriptor/myBlockDescriptor/myBlockDescriptor.h"
#include "myImageSequence/myImageSequence.h"
#include "myPlugin/myScanner/myScanner.h"
#include <fstream>
#include <iomanip>

int main(void) {
  using std::string;
  using std::vector;

  // root path for training samples
  const string sTrainingSamplesRoot = "D:/Database/Sample/01/";
  // root path for testing samples
  const string sTestingSamplesRoot = "D:/Database/Sample/01/";
  // svm model file
  const string sModelFile = "../Models/HOG_LBP_ev+ni.xml";
  const bool bTraining = false;
  const bool bTesting = true;
  Descriptor::myBlockDescriptor oDenseDetector(cv::Mat(), cv::Size2i(8, 8));
  {
    const vector<int> viDenseFeature = {
      Descriptor::myBlockDescriptor::Feature::HOG_STANDARD,
      Descriptor::myBlockDescriptor::Feature::LBP_8_1_UNIFORM
    };
    // setting dense detector
    for (auto feature : viDenseFeature) {
      oDenseDetector.EnableFeature(feature);
    }
  }
  // Rect2i for calculate features
  vector<cv::Rect2i> vrDense;
  // claculate the rect for dense extractor
  {
    Plugin::myScanner scanner(cv::Point2i(0, 0), cv::Point2i(64, 128));
    scanner.CalRect(vrDense, cv::Size2i(8, 8), cv::Point2i(8, 8));
  }
  Classifier::mySVM oSVM;

  // array saves Pos and Neg string
  const std::array<string, 2> vsPosNeg = { "Positive", "Negative" };
  const std::map<string, int> Labels = { {"Positive", +1}, {"Negative", -1} };
  // define time intervals strings
  const vector<string> vsTimeInterval = { "Morning", "Noon"/*, "Evening", "Night"*/ };

  if (bTraining) {
    for (const auto& sTime : vsTimeInterval) {
      for (const auto& sPN : vsPosNeg) {
        cv::Mat mImg;
        auto SamplesRoot = sTrainingSamplesRoot + sTime + "/" + sPN + "/";
        myImageSequence oReader(SamplesRoot, "", "bmp", false);
        while (oReader >> mImg) {
          std::cout << "\rReading " << SamplesRoot << oReader.GetSequenceNumberString();
          oDenseDetector.SetImage(mImg);
          vector<float> vfCascade;
          for (const auto& r : vrDense) {
            vector<float> vfFeature;
            oDenseDetector.Describe(r, vfFeature);
            for (auto f : vfFeature) {
              vfCascade.push_back(f);
            }
          }
          oSVM.AddSample(Labels.at(sPN), vfCascade);
        }
        std::cout << std::endl;
      }
    }
    std::cout << "Start SVM Trianing" << std::endl;
    oSVM.Train();
    oSVM.Save(sModelFile);
  } else {
    oSVM.Load(sModelFile);
  }

  if (bTesting) {
    std::cout << "Start SVM Testing" << std::endl;
    if (oSVM.IsEmpty()) {
      std::cout << "SVM is empty!!" << std::endl;
      exit(EXIT_FAILURE);
    }

    struct Score {
      unsigned int TruePositive = 0;
      unsigned int TrueNegative = 0;
      unsigned int FalsePositive = 0;
      unsigned int FalseNegative = 0;
    } score;

    std::ofstream File("Distance.txt");

    for (const auto& sTime : vsTimeInterval) {
      for (const auto& sPN : vsPosNeg) {
        cv::Mat mImg;
        auto SamplesRoot = sTrainingSamplesRoot + sTime + "/" + sPN + "/";
        myImageSequence oReader(SamplesRoot, "", "bmp", false);
        while (oReader >> mImg) {
          std::cout << "\rReading " << SamplesRoot + oReader.GetSequenceNumberString();
          oDenseDetector.SetImage(mImg);
          vector<float> vfCascade;
          for (const auto& r : vrDense) {
            vector<float> vfFeature;
            oDenseDetector.Describe(r, vfFeature);
            for (auto f : vfFeature) {
              vfCascade.push_back(f);
            }
          }
          auto Label = oSVM.Predict(vfCascade);
          auto Distance = oSVM.GetDistance(vfCascade);
          File << Label << ", " << Distance << std::endl;

          if (Labels.at(sPN) >= 0 && Label >= 0) {
            ++score.TruePositive;
          } else if (Labels.at(sPN) < 0 && Label >= 0) {
            ++score.FalsePositive;
          } else if (Labels.at(sPN) >= 0 && Label < 0) {
            ++score.FalseNegative;
          } else {
            ++score.TrueNegative;
          }
        }

        std::cout << std::endl << "Finish " << sPN << std::endl;
      }
    }

    std::cout << "TP: " << std::setw(7) << score.TruePositive << std::endl
      << "FP: " << std::setw(7) << score.FalsePositive << std::endl
      << "TN: " << std::setw(7) << score.TrueNegative << std::endl
      << "FN: " << std::setw(7) << score.FalseNegative << std::endl;
  }
  return 0;
}