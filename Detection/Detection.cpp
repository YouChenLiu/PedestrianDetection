#include <memory>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "../myLibrary/common.h"
#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/myFeatureDescriptor/myBlockDescriptor/myBlockDescriptor.h"
#include "../myLibrary/myScanner/myScanner.h"
#include "../myLibrary/myClassifier.h"
#include "../myLibrary/myPosition/myPosition.h"
#include "../myLibrary/myModelCollector/myModelCollector.h"
#include "../myLibrary/myModelIndexer/myLBPIndexer/myLBPIndexer.h"

using std::vector;
using std::string;

const cv::Size2i SampleSize(64, 128);
const cv::Size2i BlockSize(8, 8);
// feature extractor setting
Descriptor::myBlockDescriptor oMyDetector(cv::Mat(), BlockSize);

// vector for my method feature set
const vector<int> viTwoLayerFeature = {
    Descriptor::myBlockDescriptor::Feature::HOG_SINGLE_CELL |
    Descriptor::myBlockDescriptor::Feature::L2_NORM,
    Descriptor::myBlockDescriptor::Feature::LBP_8_1_UNIFORM
};
Descriptor::myBlockDescriptor oDenseDetector(cv::Mat(), BlockSize);
const vector<int> viDenseFeature = {
    Descriptor::myBlockDescriptor::Feature::HOG_STANDARD |
    Descriptor::myBlockDescriptor::Feature::L2_NORM,
    Descriptor::myBlockDescriptor::Feature::LBP_8_1_UNIFORM
};

// classifier setting
const int iCollectorCount = (((SampleSize.height - 2 * BlockSize.height) / 8) *
                             ((SampleSize.width - 2 * BlockSize.width) / 8));
vector<myModelCollector> voCollector(iCollectorCount);
std::unique_ptr<Classifier::myAdaBoost> pL2Classifier = nullptr;
std::unique_ptr<Classifier::mySVM> pDenseClassifier = nullptr;

// the positions of feature extraction left-top points
vector<cv::Point2i> vptMinePos;
vector<cv::Point2i> vptDensePos;

float DetectByMine(const cv::Mat& mImg) {
  cv::Mat mSample;
  cv::resize(mImg, mSample, SampleSize);
  oMyDetector.SetImage(mSample);
  vector<float> vfResult(voCollector.size(), -1.0f);
  myLBPIndexer oIndexr(BlockSize);
  for (size_t i = 0; i < vptMinePos.size(); ++i) {
    vector<float> vfFeature;
    auto Position = vptMinePos.at(i);
    oMyDetector.Describe(Position, vfFeature);
    auto iIndex = oIndexr.GetBinNumber(mSample, Position);
    auto fResult = voCollector.at(i).Predict(iIndex, vfFeature);
    vfResult.at(i) = fResult;
  }

  return pL2Classifier->Predict(vfResult);
}

float DetectByDense(const cv::Mat& mImg) {
  cv::Mat mSample;
  cv::resize(mImg, mSample, SampleSize);
  oDenseDetector.SetImage(mSample);
  vector<float> vfCascade;
  for (size_t i = 0; i < vptDensePos.size(); ++i) {
    vector<float> vfFeature;
    oDenseDetector.Describe(vptDensePos.at(i), vfFeature);
    for (auto f : vfFeature) {
      vfCascade.push_back(f);
    }
  }
  return pDenseClassifier->Predict(vfCascade);
  
}

int main(void) {
  const string sTestingImgRoot = "C:/Users/youch/Desktop/Waiting/";

  vector<string> vsFolder = {
    "2015-1006_1335-1345_06/",
    "2015-1005_2120-2130_07/",
    "2015-1015_1715-1725_04/",
    "2015-1007_2120-2130_02/"
  };
  vector<int> viFirstNum = {
    6184,
    8258,
    6669,
    2302
  };
  // root path for testing samples
  const string sL2Model = "A_L2_60_SingleCell.xml";
  const string sModelName = "SingleCell_Models";
  const string sDenseModel = "Dense.xml";

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
  vector<cv::Rect2i> vWindow;
  {
    cv::Size2i ImgSize(640, 480);
    cv::Size2i RectSize(32, 90);
    cv::Point2i Interval(8, 16);
    myScanner scanner(ImgSize, RectSize);
    scanner.CalRect(vWindow, cv::Point2i(0, 200), cv::Point2i(640, 480), Interval);
  }
  // read trained models
  {
    // build mine classifier
    std::cout << "Reading saved L1 models" << std::endl;
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
    std::cout << "Reading saved L2 models" << std::endl;
    pL2Classifier = std::make_unique<Classifier::myAdaBoost>(sL2Model);
    
    // build dense classifier
    std::cout << "Reading dense models" << std::endl;
    pDenseClassifier = std::make_unique<Classifier::mySVM>(sDenseModel);
    //pDenseClassifier->Load(sDenseModel);
  }
  
  // claculate the position for my extractor
  {
    cv::Point2i ptStart(BlockSize.width, BlockSize.height);
    cv::Point2i ptEnd(SampleSize.width - BlockSize.width,
                      SampleSize.height - BlockSize.height);
    cv::Point2i ptStep(BlockSize.width, BlockSize.height);
    Plugin::myPosition(ptStart, ptEnd).CalPositions(vptMinePos, ptStep);
  }
  // claculate the position for my extractor
  {
    cv::Point2i ptStart(0, 0);
    cv::Point2i ptEnd(SampleSize.width, SampleSize.height);
    cv::Point2i ptStep(BlockSize.width, BlockSize.height);
    Plugin::myPosition(ptStart, ptEnd).CalPositions(vptDensePos, ptStep);
  }

  for (size_t i = 0; i < vsFolder.size(); ++i) {
    string sFolder = vsFolder.at(i);
    sFolder.resize(sFolder.size() - 1);
    cv::VideoWriter DenseWriter(sFolder + "_Dense.avi", CV_FOURCC('F', 'M', 'P', '4'), 30, cv::Size2i(640, 480));
    cv::VideoWriter MineWriter(sFolder + "_Mine.avi", CV_FOURCC('F', 'M', 'P', '4'), 30, cv::Size2i(640, 480));
    myImageSequence oReader(sTestingImgRoot + vsFolder.at(i), "", "bmp", false);
    oReader.SetAttribute(myImageSequence::Attribute::FIRST_NUMBER, viFirstNum.at(i));
    cv::Mat mImg;
    while (oReader >> mImg) {
      vector<cv::Mat> vImg(3, mImg);
      cv::Mat mMineResult;
      cv::merge(vImg, mMineResult);
      cv::Mat mDenseResult = mMineResult.clone();
      for (auto r : vWindow) {
        auto fMineResult = DetectByMine(mImg(r));
        if (fMineResult >= 0) {
          cv::rectangle(mMineResult, r, MineColor);
        }
        auto fDenseResult = DetectByDense(mImg(r));
        if (fDenseResult >= 0) {
          cv::rectangle(mDenseResult, r, DenseColor);
        }
      }
      cv::imshow("Mine", mMineResult);
      MineWriter << mMineResult;
      cv::imshow("Dense", mDenseResult);
      DenseWriter << mDenseResult;
      cv::waitKey(1);
    }
    cv::destroyAllWindows();
  }
  return 0;
}