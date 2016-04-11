#include "../myLibrary/myClassifier/mySVM/mySVM.h"
#include "../myLibrary/myClassifier/myAdaBoost/myAdaBoost.h"
#include "../myLibrary/myModelCollector/myModelCollector.h"
#include "../myLibrary/myFeatureDescriptor/myBlockDescriptor/myBlockDescriptor.h"
#include "../myLibrary/myModelIndexer/myLBPIndexer/myLBPIndexer.h"
#include "../myLibrary/myImageSequence/myImageSequence.h"
#include <opencv2/highgui.hpp>
#include <fstream>

int main(void) {
  using std::string;
  using std::vector;

  // root path for training samples
  const string sTrainingSamplesRoot = "D:/Database/01/";
  // root path for testing samples
  const string sTestingSamplesRoot = "D:/Database/02/";
  // determind do training or testing
  const bool bTrainingL1 = false;
  const bool bTrainingL2 = false;
  const bool bTesting = true;
  
  const cv::Size2i ImgSize(64, 128);
  const cv::Size2i BlockSize(8, 8);
  const int iCollectorCount = (((ImgSize.height - 2 * BlockSize.height) / 8) *
                               ((ImgSize.width - 2 * BlockSize.width) / 8));

  Classifier::mySupervisedClassifier* oL2Classifier = new Classifier::myAdaBoost(60);
  const string sL2Model = "A_L2_70.xml";
  const string sModelName = "LIN_Models";

  const bool bSaving = false;

  // vector of collectors
  vector<myModelCollector> voCollector(iCollectorCount);
  for (auto& o : voCollector) {
    o.Resize(59);
  }

  myLBPIndexer oIndexr(BlockSize);

  // define time intervals strings
  const vector<string> vsTimeInterval = { "Morning", "Noon", "Evening", "Night" };
  // array saves Pos and Neg string
  const std::array<string, 2> vsPosNeg = { "Positive", "Negative" };
  // array saves labels for pos and neg
  const std::array<int, 2> viAnswer = { +1, -1 };
  // vector for feature set
  const vector<int> viFeature = {
      Descriptor::myBlockDescriptor::Feature::HOG_STANDARD,
      Descriptor::myBlockDescriptor::Feature::LBP_8_1_UNIFORM
  };
  
  // feature extractor
  Descriptor::myBlockDescriptor oExtractor(cv::Mat(), BlockSize);
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
    for (auto sTime : vsTimeInterval) {
      for (size_t i = 0; i < vsPosNeg.size(); ++i) {
        string sSamplePath = sTrainingSamplesRoot + sTime + "/" + vsPosNeg.at(i) + "/";
        myImageSequence oReader(sSamplePath, "", "bmp", false);
        cv::Mat mImg;
        while (oReader >> mImg) {
          std::cout << "\rReading " + sTime + "-" + vsPosNeg.at(i) + ":" + oReader.GetSequenceNumberString();
          oExtractor.SetImage(mImg);

          for (int y = BlockSize.height, iPos = 0; y < mImg.rows - BlockSize.height; y += BlockSize.height) {
            for (int x = BlockSize.width; x < mImg.cols - BlockSize.width; x += BlockSize.width, ++iPos) {
              vector<float> vfFeature;
              cv::Point2i Position(x, y);
              oExtractor.Describe(Position, vfFeature);
              auto iIndex = oIndexr.GetBinNumber(mImg, Position);
              voCollector.at(iPos).AddSample(iIndex, viAnswer.at(i), vfFeature);
            }
          }
        }
        std::cout << std::endl;
      }
    }

    // train and save all layer 1 models
    std::ofstream ModelList(sModelName + ".txt");
    ModelList << voCollector.size() << std::endl;
    for (std::size_t i = 0; i < voCollector.size(); ++i) {
      std::cout << "\rTraining model collector : " << i << " / " << iCollectorCount - 1;
      voCollector.at(i).TrainModels();
      ModelList << voCollector.at(i).SaveModels(sModelName) << std::endl;
    }
  } else {
    std::cout << "Reading saved models" << std::endl;
    std::ifstream ModelList(sModelName + ".txt");
    int iModelsCount = 0;
    ModelList >> iModelsCount;
    for (std::size_t i = 0; i < iModelsCount; ++i) {
      std::string sPath;
      ModelList >> sPath;
      voCollector.at(i).LoadModels(sPath);
      std::cout << "Reading models : " << i << " / " << iModelsCount - 1 << "\r";
    }
    std::cout << std::endl;
  }

  if (bTrainingL2) {
    std::cout << std::endl << "Training Layer 2 Classifier" << std::endl;
    for (auto sTime : vsTimeInterval) {
      for (size_t i = 0; i < vsPosNeg.size(); ++i) {
        std::string sSamplePath = sTrainingSamplesRoot + sTime + "/" + vsPosNeg.at(i) + "/";
        myImageSequence oReader(sSamplePath, "", "bmp", false);
        cv::Mat mImg;
        while (oReader >> mImg) {
          std::cout << "\rReading " + sTime + "-" + vsPosNeg.at(i) + ":" + oReader.GetSequenceNumberString();
          oExtractor.SetImage(mImg);
          vector<float> vfResult(iCollectorCount, 0.0f);
          for (int y = BlockSize.height, iPos = 0; y < mImg.rows - BlockSize.height; y += BlockSize.height) {
            for (int x = BlockSize.width; x < mImg.cols - BlockSize.width; x += BlockSize.width, ++iPos) {
              vector<float> vfFeature;
              cv::Point2i Position(x, y);
              oExtractor.Describe(Position, vfFeature);
              auto iIndex = oIndexr.GetBinNumber(mImg, Position);
              auto fResult = voCollector.at(iPos).Predict(iIndex, vfFeature);
              if (fResult == NAN) {
                fResult = -1.0f;
              }
              vfResult.at(iPos) = fResult;
            }
          }
          oL2Classifier->AddSample(viAnswer.at(i), vfResult);
        }
        std::cout << std::endl;
      }
    }

    oL2Classifier->Train();
    oL2Classifier->Save(sL2Model);
    std::cout << "Training Finish" << std::endl;
  } else {
    std::cout << "Load L2 Classifier" << std::endl;
    oL2Classifier->Load(sL2Model);
  }

  if (bTesting) {
    system("mkdir \"Wrong\"");
    std::ofstream ListFile(sModelName + "_WRONG.txt");
    for (auto sTime : vsTimeInterval) {
      for (size_t i = 0; i < vsPosNeg.size(); ++i) {
        string sSamplePath = sTestingSamplesRoot + sTime + "/" + vsPosNeg.at(i) + "/";
        myImageSequence oReader(sSamplePath, "", "bmp", false);
        cv::Mat mImg;
        while (oReader >> mImg) {
          std::cout << "\rReading " + sTime + "-" + vsPosNeg.at(i) + ":" + oReader.GetSequenceNumberString();
          oExtractor.SetImage(mImg);
          vector<float> vfResult(iCollectorCount, 0.0f);
          for (int y = BlockSize.height, iPos = 0; y < mImg.rows - BlockSize.height; y += BlockSize.height) {
            for (int x = BlockSize.width; x < mImg.cols - BlockSize.width; x += BlockSize.width, ++iPos) {
              vector<float> vfFeature;
              cv::Point2i Position(x, y);
              oExtractor.Describe(Position, vfFeature);
              auto iIndex = oIndexr.GetBinNumber(mImg, Position);
              auto fResult = voCollector.at(iPos).Predict(iIndex, vfFeature);
              vfResult.at(iPos) = fResult;
            }
          }
          auto DetectingResult = oL2Classifier->Predict(vfResult);
          string sResult = "\n";
          if (DetectingResult == viAnswer.at(i)) {
            if (viAnswer.at(i) == 1) {
              ++score.TruePositive;
            } else {
              ++score.TrueNegative;
            }
          } else {
            string sPN;

            if (viAnswer.at(i) == 1) {
              sPN = "pos";
              ++score.FalseNegative;
            } else {
              sPN = "neg";
              ++score.FalsePositive;
            }
            if (bSaving) {
              cv::imwrite("Wrong/" + sPN + oReader.GetSequenceNumberString() + ".jpg", mImg);
            }
            sResult = sPN + oReader.GetSequenceNumberString() + sResult;
          }
          ListFile << sResult;
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