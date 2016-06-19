#include "common.h"
#include <iomanip>
#include <opencv2/imgproc.hpp>
#include "myImageSequence/myImageSequence.h"
#include "myFeatureDescriptor/myBlockDescriptor/myBlockDescriptor.h"
#include "myClassifier.h"
#include "myPlugin.h"
#include "myModelCollector/myModelCollector.h"
#include "myModelIndexer/myLBPIndexer/myLBPIndexer.h"

using std::vector;
using std::string;

// feature extractor setting
Descriptor::myBlockDescriptor oMyDetector(cv::Mat(), cv::Size2i(8, 8));
// vector for my method feature set
const vector<int> viTwoLayerFeature = {
    Descriptor::myBlockDescriptor::Feature::HOG_SINGLE_CELL |
    Descriptor::myBlockDescriptor::Feature::L2_NORM,
    Descriptor::myBlockDescriptor::Feature::LBP_8_1_UNIFORM
};

Descriptor::myBlockDescriptor oDenseDetector(cv::Mat(), cv::Size2i(8, 8));
const vector<int> viDenseFeature = {
    Descriptor::myBlockDescriptor::Feature::HOG_STANDARD |
    Descriptor::myBlockDescriptor::Feature::L2_NORM,
    Descriptor::myBlockDescriptor::Feature::LBP_8_1_UNIFORM
};
// model constoller
vector<myModelCollector> voCollector;
// layer 2 classifer
std::unique_ptr<Classifier::myAdaBoost> pL2Classifier = nullptr;
// dense classifier
std::unique_ptr<Classifier::mySVM> pDenseClassifier = nullptr;
// the regions of feature extraction
vector<cv::Rect2i> vrMine;
vector<cv::Rect2i> vrDense;

float DetectByMine(const cv::Mat& mImg) {
  oMyDetector.SetImage(mImg);
  vector<float> vfResult(voCollector.size(), -1.0f);
  myLBPIndexer oIndexr;
  static auto Indexes = pL2Classifier->GetIndicate();
  for (auto idx : Indexes) {
    vector<float> vfFeature(68, -1.0f);
    oMyDetector.Describe(vrMine.at(idx), vfFeature);
    auto iIndex = oIndexr.GetBinNumber(mImg, vrMine.at(idx));
    auto fResult = voCollector.at(idx).Predict(iIndex, vfFeature);
    vfResult.at(idx) = fResult;
  }

  return pL2Classifier->GetWeightedSum(vfResult);
}

float DetectByDense(const cv::Mat& mImg, float& label) {
  oDenseDetector.SetImage(mImg);
  vector<float> vfCascade;
  for (size_t i = 0; i < vrDense.size(); ++i) {
    vector<float> vfFeature;
    oDenseDetector.Describe(vrDense.at(i), vfFeature);
    for (auto f : vfFeature) {
      vfCascade.push_back(f);
    }
  }
  label = pDenseClassifier->Predict(vfCascade);
  return pDenseClassifier->GetDistance(vfCascade);
}

int main(void) {
  // root path for testing samples
  const string sTestingImgRoot = "D:/Database/Slice/02/";
  // model's name
  const string sModelName = "GENERAL_TEST_Model_1";
  // later 2 model's name
  const string sL2Model = "A_L2_GENERAL_100.xml";
  // thresholds for my method
  vector<float> vfThreshold;
  for (float f = -10.0f; f <= 10.0f; f += 0.5f) {
    vfThreshold.push_back(f);
  }
  // thresholds for dense method
  vector<float> vfDenseThreshold;
  for (float f = 2.0f; f <= 7.0f; f += 0.5f) {
    vfDenseThreshold.push_back(f);
  }
  // folder name and first frame number pair
  struct NameNumber {
    string sFolderName;
    int iFirstNum;
  };
  // vector of folder name and first frame number pair
  vector<NameNumber> vnnPairs = {
    { "2015-1007_0900-0910_01", 0 }
    /*{ "2015-1007_0900-0910_02", 6614 },
    { "2015-1007_0900-0910_03", 9374 },
    { "2015-1007_0900-0910_04", 11953 },
    { "2015-1007_0900-0910_05", 14305 },
    { "2015-1007_2120-2130_01", 1250 },
    { "2015-1007_2120-2130_02", 2302 },
    { "2015-1007_2120-2130_03", 3970 },
    { "2015-1007_2120-2130_04", 5686 },
    { "2015-1007_2120-2130_05", 10800 },
    { "2015-1015_1715-1725_01", 5105 },
    { "2015-1015_1715-1725_02", 6111 },
    { "2015-1015_1715-1725_03", 6669 },
    { "2015-1015_1715-1725_04", 12165 },
    { "2015-1015_1715-1725_05", 13966 }*/
  };
  {
    // claculate the rect for my extractor
    Plugin::myScanner scanner(cv::Point2i(8, 8), cv::Point2i(56, 120));
    for (int h = 8; h <= 32; h += 8) {
      for (int w = 8; w <= 16; w += 8) {
        scanner.CalRect(vrMine, cv::Size2i(w, h), cv::Point2i(8, 8));
      } // for w
    } // for h
  }
  {
    // claculate the rect for dense extractor
    Plugin::myScanner scanner(cv::Point2i(0, 0), cv::Point2i(64, 128));
    scanner.CalRect(vrDense, cv::Size2i(8, 8), cv::Point2i(8, 8));
  }
  // read trained models
  {
    
    // build mine classifier
    std::cout << "Reading saved L1 models" << std::endl;
    // model root path
    const string sModelRoot = "../Models/";
    std::ifstream ModelList(sModelRoot + sModelName + ".txt");
    if (!ModelList.is_open()) {
      std::cout << "model list error" << std::endl;
      return 0;
    }
    int iModelsCount = 0;
    ModelList >> iModelsCount;
    voCollector = vector<myModelCollector>(vrMine.size());
    for (std::size_t i = 0; i < iModelsCount; ++i) {
      std::string sPath;
      ModelList >> sPath;
      voCollector.at(i).LoadModels(sModelRoot + sPath);
      std::cout << "Reading models : " << i << " / " << iModelsCount - 1 << "\r";
    }
    std::cout << std::endl;
    std::cout << "Reading saved L2 models" << std::endl;
    pL2Classifier = std::make_unique<Classifier::myAdaBoost>(sModelRoot + sL2Model);
    if (pL2Classifier->IsEmpty()) {
      std::cout << "layer 2 classifier error" << std::endl;
      return 0;
    }
    
    // create dense model
    //pDenseClassifier = std::make_unique<Classifier::mySVM>("../Models/Dense.xml");
  }
  // setting feature extractor
  {
    // setting two layer dtector
    for (auto feature : viTwoLayerFeature) {
      oMyDetector.EnableFeature(feature);
    }
    // setting dense detector
    for (auto feature : viDenseFeature) {
      oDenseDetector.EnableFeature(feature);
    }
  }

  for (const auto& nn : vnnPairs) {
    std::cout << "Start ";
    system("time /t");
    // folder name
    string sFolder = nn.sFolderName;
    // sequence reader
    myImageSequence oReader(sTestingImgRoot + sFolder + "/", "", "bmp", false);
    oReader.SetAttribute(myImageSequence::Attribute::FIRST_NUMBER, nn.iFirstNum);
    // vector of dumper for saving diffirent threshold result with my method
    vector<Plugin::myBBDumper> voMineDumper(vfThreshold.size());
    // vector of dumper for saving diffirent threshold result with dense method
    vector<Plugin::myBBDumper> voDenseDumper(vfDenseThreshold.size());
    // loaded image
    cv::Mat mImg;
    std::ofstream Out("AllValue.txt");
    while (oReader >> mImg) {
      std::cout << "\rProcessing...\t" << sFolder << "/" << oReader.GetSequenceNumberString();
      // add new frame to all dumpers
      for (auto& o : voMineDumper) {
        o.AddFrame(oReader.GetSequenceNumber());
      }
      for (auto& o : voDenseDumper) {
        o.AddFrame(oReader.GetSequenceNumber());
      }
      
      // scaling the img
      for (float fRatio = 1.0f; fRatio <= 2.5f; fRatio *= 1.2f) {
        cv::Size2i NewSize;
        {
          cv::Size2i Size = mImg.size();
          auto iNewWidth = static_cast<int>(Size.width * fRatio);
          auto iNewHeight = static_cast<int>(Size.height * fRatio);
          NewSize = cv::Size2i(iNewWidth, iNewHeight);
        }
        cv::Mat mRescale;
        cv::resize(mImg, mRescale, NewSize);

        vector<cv::Rect2i> vScanningWindow;
        {
          cv::Size2i RectSize(64, 128);
          cv::Point2i Interval(32, 64);
          cv::Point2i ptRB(NewSize.width, NewSize.height);
          Plugin::myScanner scanner(cv::Point2i(0, NewSize.height / 4), ptRB);
          scanner.CalRect(vScanningWindow, RectSize, Interval);
        }
        
        for (auto r : vScanningWindow) {
          cv::Rect2i OriginalRect;
          {
            auto NewX = static_cast<int>(r.x / fRatio);
            auto NewY = static_cast<int>(r.y / fRatio);
            auto NewWidth = static_cast<int>(r.width / fRatio);
            auto NewHeight = static_cast<int>(r.height / fRatio);
            OriginalRect = cv::Rect2i(NewX, NewY, NewWidth, NewHeight);
          }
          
          // the weighted sum of my method
          auto fMineResult = DetectByMine(mRescale(r));
          for (size_t i = 0; i < vfThreshold.size(); ++i) {
            if (fMineResult >= vfThreshold.at(i)) {
              voMineDumper.at(i).AddRectangle(OriginalRect);
            } // if fMineResult
          } // for vfThreshold
          
          /*
          float fLabel = 0.0f;
          // raw distance of dense distance
          auto fDenseResult = DetectByDense(mRescale(r), fLabel);
          Out << fLabel << ", " << fDenseResult << std::endl;
          for (size_t i = 0; i < vfDenseThreshold.size(); ++i) {
            if (fDenseResult > vfDenseThreshold.at(i)) {
              voDenseDumper.at(i).AddRectangle(OriginalRect);
            }
          }
          */
        } // for scanning window
      } // for fRatio
    } // while

    
    for (size_t i = 0; i < vfThreshold.size(); ++i) {
      std::stringstream ss;
      ss << sFolder << "_Mine_"
         << std::fixed << std::setprecision(1) << vfThreshold.at(i)
         << ".xml";
      //voMineDumper.at(i).Save(ss.str());
    }
    
    /*
    for (size_t i = 0; i < vfDenseThreshold.size(); ++i) {
      std::stringstream ss;
      ss << sFolder << "_Dense_"
         << std::fixed << std::setprecision(1) << vfDenseThreshold.at(i)
         << ".xml";
      voDenseDumper.at(i).Save(ss.str());
    }
    */
    std::cout << "\r" << sFolder
              << "\tFinish                             "
              << std::endl;
    std::cout << "End";
    system("time /t");
  } // for vnnPairs

  return 0;
}