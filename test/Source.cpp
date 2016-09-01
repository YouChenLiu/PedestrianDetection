#include "myPlugin.h"
#include "myClassifier.h"
#include "myModelCollector/myModelCollector.h"
#include "myFeatureDescriptor/myBlockDescriptor/myBlockDescriptor.h"
#include "myImageSequence/myImageSequence.h"
#include "myModelIndexer/myLBPIndexer/mylbpindexer.h"
#include "myPlugin/myBBReader/myBBReader.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <iomanip>
#include "myVerifier/myFPPW/myFPPW.h"

void DrawRect(std::vector<cv::Mat>& vmDrawing, const std::string& sFilePath) {
  std::vector<int> viResult;
  {
    std::ifstream File(sFilePath);
    std::string s;
    while (std::getline(File, s)) {
      std::stringstream ss(s);
      int i = 0;
      ss >> i;
      viResult.push_back(i);
    }
  }
  std::vector<cv::Rect2i> vRect;
  std::vector<size_t> viScanSize;
  {
    Plugin::myScanner scanner(cv::Point2i(8, 8), cv::Point2i(56, 120));
    for (int h = 8, i = 0; h <= 32; h += 8) {
      for (int w = 8; w <= 16; w += 8, ++i) {
        scanner.CalRect(vRect, cv::Size2i(w, h), cv::Point2i(8, 8));
        viScanSize.push_back(vRect.size() - 1);
      } // for w
    } // for h
  }

  {
    
    vmDrawing.reserve(viScanSize.size());
    for (size_t i = 0; i < viScanSize.size(); ++i) {
      cv::Size2i Size = vRect.at(viScanSize.at(i)).size();
      std::stringstream ss;
      ss << Size.width << "x" << Size.height;
      cv::Mat mTemp = cv::Mat::zeros(128, 64, CV_8UC1);
      cv::putText(mTemp, ss.str(), cv::Point2i(10, 10), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar::all(255));
      vmDrawing.push_back(mTemp.clone());
    }
  }
  for (auto RectIdx : viResult) {
    size_t iIndex = 0;
    for (auto idx : viScanSize) {
      if (idx < RectIdx) {
        ++iIndex;
      }
    }
    cv::rectangle(vmDrawing.at(iIndex), vRect.at(RectIdx), cv::Scalar::all(255));
  }
}

void MergeResult(const std::string& sName) {
  cv::VideoCapture Dense(sName + "_Dense.avi");
  cv::VideoCapture Mine(sName + "_Mine.avi");
  cv::VideoWriter Merging(sName + ".avi", CV_FOURCC('F', 'M', 'P', '4'), 15, cv::Size2i(1280, 480));

  cv::Mat mDense;
  cv::Mat mMine;
  cv::Mat mImg = cv::Mat::zeros(cv::Size2i(1280, 480), CV_8UC3);

  while (true) {
    Dense >> mDense;
    Mine >> mMine;
    if (mDense.empty() | mMine.empty()) {
      break;
    }

    mDense.copyTo(mImg(cv::Rect2i(0, 0, 640, 480)));
    mMine.copyTo(mImg(cv::Rect2i(640, 0, 640, 480)));
    Merging << mImg;
  }
}

int main(int argc, char* argv[]) {
  // find the all selected number in OpenCV AdaBoost xml file
  /*
  std::vector<cv::Mat> vmDrawing;
  DrawRect(vmDrawing, "A_L2_GENERAL_100_selection.txt");
  for (size_t i = 0; i < vmDrawing.size(); ++i) {
    std::stringstream ss;
    ss << i << ".jpg";
    cv::imwrite(ss.str(), vmDrawing.at(i));
  }
  */

  // calculate ROC
  /*
  struct NameNumber {
    std::string sFolderName;
    int iFirstNum;
  };

  std::vector<NameNumber> vnnPairs = {
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
  
  std::vector<float> vfThreshold;
  for (float f = -10.0f; f <= 20.0f; f += 0.5f) {
    vfThreshold.push_back(f);
  }
  for (const auto& nn : vnnPairs) {
    // create file for writing computation result
    std::ofstream ROCText(nn.sFolderName + ".txt");
    ROCText << nn.sFolderName << std::endl;
    ROCText << "Dense" << std::endl
      << "Th\tFPPW\t\tMissRate" << std::endl;
    Verifier::myFPPW fppw(1668, "../ClassificationResult/" + nn.sFolderName + ".xml",
                          "Result/" + nn.sFolderName + "_Dense.xml");
    fppw.CompareAllFrames();
    ROCText << 0 << "\t"
      << fppw.GetResult() << "\t"
      << fppw.GetMissRate() << std::endl;
    
    ROCText << "Th\tFPPW\t\tMissRate" << std::endl;
    for (size_t i = 0; i < vfThreshold.size(); ++i) {
      std::stringstream ss;
      ss << "../ClassificationResult/" << nn.sFolderName << "_Mine_"
        << std::fixed << std::setprecision(1) << vfThreshold.at(i)
        << ".xml";

      Verifier::myFPPW fppw(1668, "../GroundTruth/" + nn.sFolderName + ".xml", ss.str());
      fppw.CompareAllFrames();
      ROCText << vfThreshold.at(i) << "\t"
        << fppw.GetResult() << "\t"
        << fppw.GetMissRate() << std::endl;
    }
  }
  */
  
  // test the SVM
  srand(time(0));
  Classifier::mySVM oSVM;
  for (int i = 0; i < 10000; ++i) {
    std::vector<float> vfPos;
    std::vector<float> vfNeg;
    for (int j = 0; j < 100; ++j) {
      vfPos.push_back(+(rand() % 20) + 5);
      vfNeg.push_back(-(rand() % 20) - 5);
    }
    oSVM.AddSample(+1, vfPos);
    oSVM.AddSample(-1, vfNeg);
  }
  
  oSVM.Train();

  for (int i = 0; i < 30; ++i) {
    std::vector<float> vfFeature;
    for (int j = 0; j < 100; ++j) {
      vfFeature.push_back(+(rand() % 50) - 25);
    }
    auto label = oSVM.Predict(vfFeature);
    auto distance = oSVM.GetDistance(vfFeature);
    std::cout << "Label: " << (label > 0 ? "+" : "-") << ", " << distance << std::endl;
  }
  return 0;
}