#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")   // link libaray

int main(void) {
  const std::string sCascadePath = "data/cascade.xml";
  cv::CascadeClassifier oCascadeClassifier;
  if (!oCascadeClassifier.load(sCascadePath)) {
    std::cout << "Loading Cascade Classifier XML File Faild!!" << std::endl;
    exit(EXIT_FAILURE);
  }

  cv::VideoCapture Video("TestVideo.avi");
  if (!Video.isOpened()) {
    std::cout << "Loading Video File Faild!!" << std::endl;
    exit(EXIT_FAILURE);
  }

  // region of intresting
  // the detector will process only this range
  const cv::Rect ROIRect(0, 100, 640, 320);
  
  while (true) {
    cv::Mat mImg;
    Video.read(mImg);
    if (mImg.empty()) {
      std::cout << "Reading Image ERROR!!" << std::endl;
      break;
    }

    // setting ROI
    cv::Mat mROI = mImg(ROIRect);
    // vector for saving detected result
    std::vector<cv::Rect> vPedestrian;
    // dectect by cascade
    oCascadeClassifier.detectMultiScale(mROI,                 // detected image
                                        vPedestrian,          // output the result
                                        1.05,                 // scaling factor
                                        1,                    // minNeighbors
                                        0,                    // flag
                                        cv::Size2i(30, 40));  // minSize

    // draw bounding box
    for (auto p : vPedestrian) {
      p.y += ROIRect.y;
      cv::rectangle(mImg, p, cv::Scalar::all(255));
    }

    // play sound if deteced
    if (vPedestrian.size() > 3) {
      PlaySound(L"beep.wav", NULL, SND_ASYNC);
    }

    // draw ROI
    cv::rectangle(mImg, ROIRect, cv::Scalar::all(255));

    // show image
    cv::imshow("result", mImg);

    // press Esc
    if (cv::waitKey(1) == 27) {
      break;
    }
  }

  return 0;
}