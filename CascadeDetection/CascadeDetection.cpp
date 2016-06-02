#include <cstdlib>
#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")   // link libaray

int main(void) {
  std::string sCascadePath = "data/cascade.xml";
  cv::CascadeClassifier oCascadeClassifier;
  if (!oCascadeClassifier.load(sCascadePath)) {
    std::cout << "Loading Cascade Classifier XML File Faild!!" << std::endl;
    exit(EXIT_FAILURE);
  }

  // open video adapter
  cv::VideoCapture oReader(1);
  if (!oReader.isOpened()) {
    std::cout << "Open Camera ERROR!!" << std::endl;
  }

  const cv::Rect ROIRect(0, 0, 640, 480);

  while (true) {
    cv::Mat mImg;
    oReader.read(mImg);
    // check image
    if (mImg.empty()) {
      std::cout << "Reading Image ERROR!!!" << std::endl;
      break;
    }
    
    // setting ROI
    cv::Mat mROI = mImg(ROIRect);
    // vector for saving detected result
    std::vector<cv::Rect> vPedestrian;
    // dectect by cascade
    oCascadeClassifier.detectMultiScale(mROI, vPedestrian, 1.5, 5, 0);

    // draw bounding box
    for (auto p : vPedestrian) {
      p.y += ROIRect.y;
      cv::rectangle(mImg, p, cv::Scalar::all(255));
    }

    // play sound if deteced
    if (vPedestrian.size() > 3) {
      PlaySound(L"beep.wav", NULL, SND_ASYNC);
    }

    cv::line(mImg, cv::Point2i(ROIRect.x, ROIRect.y), cv::Point2i(ROIRect.width, ROIRect.y), cv::Scalar::all(255));
    cv::line(mImg, cv::Point2i(ROIRect.x, ROIRect.y + ROIRect.height), cv::Point2i(ROIRect.width, ROIRect.y + ROIRect.height), cv::Scalar::all(255));

    cv::imshow("result", mImg);

    // press Esc
    if (cv::waitKey(1) == 27) {
      break;
    }
  }

  return 0;
}