#include "../myLibrary/myPlugin.h"
#include "../myLibrary/myModelIndexer/myLBPIndexer/mylbpindexer.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(void) {
  myLBPIndexer Indexer;
  auto index = Indexer.GetBinNumber(cv::Mat(128, 64, CV_8UC1), cv::Rect2i(cv::Point2i(8, 64), cv::Size2i(32, 64)));
  
  
  std::vector<cv::Rect2i> vRect;
  Plugin::myScanner scanner(cv::Point2i(8, 8), cv::Point2i(56, 120));
  scanner.CalRect(vRect, cv::Size2i(32, 64), cv::Point2i(8, 8));
  cv::Mat mImg = cv::Mat::zeros(128, 64, CV_8UC1);
  for (const auto& r : vRect) {
    cv::rectangle(mImg, r, cv::Scalar::all(255));
  }
  cv::imshow("", mImg);
  cv::waitKey(0);
  
  return 0;
}