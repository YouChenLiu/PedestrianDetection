#include "myPlugin.h"
#include "myClassifier.h"
#include "myModelCollector/myModelCollector.h"
#include "myFeatureDescriptor/myBlockDescriptor/myBlockDescriptor.h"
#include "myImageSequence/myImageSequence.h"
#include "myModelIndexer/myLBPIndexer/mylbpindexer.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <iomanip>

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
  std::vector<cv::Mat> vmDrawing;
  DrawRect(vmDrawing, "A_L2_GENERAL_100_selection.txt");
  for (size_t i = 0; i < vmDrawing.size(); ++i) {
    std::stringstream ss;
    ss << i << ".jpg";
    cv::imwrite(ss.str(), vmDrawing.at(i));
  }
  return 0;
}