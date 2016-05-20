#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"
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

// classifier setting
  
vector<myModelCollector> voCollector;
std::unique_ptr<Classifier::myAdaBoost> pL2Classifier = nullptr;
std::unique_ptr<Classifier::mySVM> pDenseClassifier = nullptr;

// the positions of feature extraction left-top points
vector<cv::Rect2i> vrMine;
vector<cv::Rect2i> vrDense;

float DetectByMine(const cv::Mat& mImg) {
  oMyDetector.SetImage(mImg);
  vector<float> vfResult(voCollector.size(), -1.0f);
  myLBPIndexer oIndexr;
  for (size_t i = 0; i < vrMine.size(); ++i) {
    vector<float> vfFeature(68, -1.0f);
    oMyDetector.Describe(vrMine.at(i), vfFeature);
    auto iIndex = oIndexr.GetBinNumber(mImg, vrMine.at(i));
    auto fResult = voCollector.at(i).Predict(iIndex, vfFeature);
    vfResult.at(i) = fResult;
  }

  return pL2Classifier->Predict(vfResult);
}

float DetectByDense(const cv::Mat& mImg) {
  oDenseDetector.SetImage(mImg);
  vector<float> vfCascade;
  for (size_t i = 0; i < vrDense.size(); ++i) {
    vector<float> vfFeature;
    oDenseDetector.Describe(vrDense.at(i), vfFeature);
    for (auto f : vfFeature) {
      vfCascade.push_back(f);
    }
  }
  return pDenseClassifier->Predict(vfCascade);
  
}

int main(void) {
  const string sTestingImgRoot = "C:/Users/youch/Desktop/02/";

  struct NameNumber {
    string sFolderName;
    int iFirstNum;
  };

  vector<NameNumber> vnnPairs = {
    { "2015-1007_0900-0910_01", 0 },
    { "2015-1007_0900-0910_02", 6614 },
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
    { "2015-1015_1715-1725_05", 13966 }
  };

  // root path for testing samples
  const string sL2Model = "A_L2_GENERAL_100.xml";
  const string sModelName = "GENERAL_TEST_Model_1";
  const string sDenseModel = "Dense.xml";

  // claculate the rect for my extractor
  {
    Plugin::myScanner scanner(cv::Point2i(8, 8), cv::Point2i(56, 120));
    for (int h = 8; h <= 32; h += 8) {
      for (int w = 8; w <= 16; w += 8) {
        scanner.CalRect(vrMine, cv::Size2i(w, h), cv::Point2i(8, 8));
      } // for w
    } // for h
  }
  // claculate the rect for dense extractor
  {
    Plugin::myScanner scanner(cv::Point2i(0, 0), cv::Point2i(64, 128));
    scanner.CalRect(vrDense, cv::Size2i(8, 8), cv::Point2i(8, 8));
  }

  cv::Scalar MineColor(0, 255, 0);
  cv::Scalar DenseColor(0, 0, 255);

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
  
  // read trained models
  {
    // build mine classifier
    std::cout << "Reading saved L1 models" << std::endl;
    std::ifstream ModelList(sModelName + ".txt");
    int iModelsCount = 0;
    ModelList >> iModelsCount;
    voCollector = vector<myModelCollector>(vrMine.size());
    for (std::size_t i = 0; i < iModelsCount; ++i) {
      std::string sPath;
      ModelList >> sPath;
      voCollector.at(i).LoadModels(sPath);
      std::cout << "Reading models : " << i << " / " << iModelsCount - 1 << "\r";
    }
    std::cout << std::endl;
    std::cout << "Reading saved L2 models" << std::endl;
    pL2Classifier = std::make_unique<Classifier::myAdaBoost>(sL2Model);
    
    // build dense classifier
    std::cout << "Reading dense models" << std::endl;
    pDenseClassifier = std::make_unique<Classifier::mySVM>(sDenseModel);
    
  }

  for (const auto nn : vnnPairs) {
    string sFolder = nn.sFolderName;
    cv::VideoWriter oWriter(sFolder + ".avi", CV_FOURCC('F', 'M', 'P', '4'), 10, cv::Size2i(1280, 480));
    myImageSequence oReader(sTestingImgRoot + sFolder + "/", "", "bmp", false);
    oReader.SetAttribute(myImageSequence::Attribute::FIRST_NUMBER, nn.iFirstNum);
    Plugin::myBBDumper oMineDumper;
    Plugin::myBBDumper oDenseDumper;
    cv::Mat mImg;
    while (oReader >> mImg) {
      std::cout << "\rProcessing...\t" << sFolder << "/" << oReader.GetSequenceNumberString();
      oMineDumper.AddFrame(oReader.GetSequenceNumber());
      oDenseDumper.AddFrame(oReader.GetSequenceNumber());
      vector<cv::Mat> vImg(3, mImg);
      cv::Mat mMineResult;
      cv::merge(vImg, mMineResult);
      cv::Mat mDenseResult = mMineResult.clone();
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
          
          auto fMineResult = DetectByMine(mRescale(r));
          if (fMineResult >= 0) {
            cv::rectangle(mMineResult, OriginalRect, MineColor);
            oMineDumper.AddRectangle(OriginalRect);
          }
          auto fDenseResult = DetectByDense(mRescale(r));
          if (fDenseResult >= 0) {
            cv::rectangle(mDenseResult, OriginalRect, DenseColor);
            oDenseDumper.AddRectangle(OriginalRect);
          }
        }
      } // for fRatio
      // draw the result
      cv::Mat mResult = cv::Mat::zeros(480, 1280, CV_8UC3);
      mDenseResult.copyTo(mResult(cv::Rect2i(0, 0, 640, 480)));
      mMineResult.copyTo(mResult(cv::Rect2i(640, 0, 640, 480)));
      cv::imshow("Result", mResult);
      oWriter << mResult;
      cv::waitKey(1);
    } // while
    cv::destroyAllWindows();
    std::cout << "\r" << sFolder << "\tFinish" << std::endl;
    oMineDumper.Save(sFolder + "_Mine.xml");
    oDenseDumper.Save(sFolder + "_Dense.xml");
  } // for vnnPairs
  return 0;
}