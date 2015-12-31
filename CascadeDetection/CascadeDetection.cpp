//#define _VIDEO_

#ifndef _VIDEO_
#   define _SEQUENCE_
#endif

#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ml.hpp>
#include "../myLibrary/myImageSequence/myImageSequence.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

const int iFirstNum = 0;
const cv::Size2i BlockSize(8, 8);
int main(void) {
    std::string sCascadePath = "data/cascade.xml";
    cv::CascadeClassifier oCascadeClassifier;
    if (!oCascadeClassifier.load(sCascadePath)) {
        std::cout << "Loading Cascade Classifier Faild!" << std::endl;
        exit(EXIT_FAILURE);
    }

#   ifdef _SEQUENCE_
    std::string sName = "2015-1005_0900-0915/01";
        myImageSequence oReader(std::string("D:/Database/spilt/") + sName + "/", "", "bmp");
        oReader.SetAttribute(myImageSequence::Attribute::FIRST_NUMBER, 4253);
        oReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
#   else
        cv::VideoCapture oReader(1);
        if (!oReader.isOpened()) {
            std::cout << "Open Camera" << std::endl;
        }
#   endif

    cv::VideoWriter oOutput(sName + ".avi", CV_FOURCC('F', 'M', 'P', '4'), 60, cv::Size2i(384, 288));
    cv::Mat mImg;
    int i = iFirstNum;
    //const cv::Rect ROIRect(0, 90, 640, 205);
    const cv::Rect ROIRect(0, 0, 640, 480);
    while (true) {
#       ifdef _VIDEO_
            oReader.read(mImg);
#       else
            oReader >> mImg;
#       endif
        if (mImg.empty()) {
            std::cout << "Reading Image!" << std::endl;
            break;
        }
        cv::Mat mROI = mImg(ROIRect);
        
        std::vector<cv::Rect> vPedestrian;
        oCascadeClassifier.detectMultiScale(mROI, vPedestrian, 1.5, 5, 0);
        
        int iPedestranNum = 0;
        for (auto p : vPedestrian) {
            //cv::Mat mHypothesis = mROI(p);
            ++iPedestranNum;
            p.y += ROIRect.y;
            cv::rectangle(mImg, p, cv::Scalar::all(255));
        }
        
        if (iPedestranNum > 3) {
            //PlaySound(L"beep.wav", NULL, SND_ASYNC);
        }
        
        cv::line(mImg, cv::Point2i(ROIRect.x, ROIRect.y), cv::Point2i(ROIRect.width, ROIRect.y), cv::Scalar::all(255));
        cv::line(mImg, cv::Point2i(ROIRect.x, ROIRect.y + ROIRect.height), cv::Point2i(ROIRect.width, ROIRect.y + ROIRect.height), cv::Scalar::all(255));

        //cv::putText(mImg, std::to_string(i), cv::Point2i(30, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255, 0));
        //oOutput << mImg;
        
        cv::imshow("result", mImg);
        
        if (cv::waitKey(1) == 'c') {
            break;
        }
        
    }

    //oOutput.release();
    return 0;
}