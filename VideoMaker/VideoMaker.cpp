/*
 *  The key function of this application is reading images and join them to a sequence.
 *  Before use this application.
 *  You will need to modify the project properties and
 *  source code to let it fit your environment and situation.
 *  Setup the include path, library path and OpenCV environment.
 *  Modify the global variables and the string format in sprintf function.
 *  After all, just glad for use.
*/

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "../myLibrary/myImageSequence/myImageSequence.h"

// parameters for reading images
const bool bIsReadingColorImage = false;            // determining the input images is colorful or not
const std::string sImageRootPath =                  // root path for source images
"D:/Database/car01/";
const std::string sImageNamePrefix = "";            // the image name before sequence number, like prefix1234.bmp
const int iFirstSequenceNumber = 2827;                 // number for the first image
const int iStopNumber = 3160;
const char cPaddingCharacter = '0';                 // padding character. it will put down after the image name with the specific length
const int iPaddingLength = 7;                       // padding length, ex: 4 for 0000.bmp, 0001.bmp ...etc.
                                                    // if set to 0, it means not need padding, like 0.bmp, 1.bmp ...etc.
const std::string sFilenameExtension = "bmp";            // file extension name for image, ex: bmp, jpg

//parameters for encoding video
const std::string sVideoPath =                      // video path
sImageRootPath + "Thermal01.avi";
const char sCodec[] = "FMP4";                       // codec name "FMP4" for ffmpeg
const double fFPS = 60;                             // FPS

// parameters for some features in this application
const bool bIsShowingResult = false;                 // showing the background subtraction result when running
const bool bIsPuttingText = true;                  // burn the sequence number at left-top

int main(void) {
    cv::VideoWriter videoWriter;
    myImageSequence oReader(sImageRootPath, sImageNamePrefix, sFilenameExtension, bIsReadingColorImage);
    cv::Mat mImage;

    oReader.SetAttribute(myImageSequence::Attribute::FIRST_NUMBER, iFirstSequenceNumber);
    oReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, iPaddingLength);
    oReader.SetAttribute(myImageSequence::Attribute::PADDING_CHARACTER, cPaddingCharacter);

    while (oReader >> mImage) {
        // burning sequence number on result image
        if (bIsPuttingText) {
            cv::putText(mImage, oReader.GetSequenceNumberString(), cv::Point2i(30, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255, 0));
        }

        // writing out video frame
        if (videoWriter.isOpened() == false) {
            videoWriter.open(sVideoPath, CV_FOURCC(sCodec[0], sCodec[1], sCodec[2], sCodec[3]),
                fFPS, cv::Size2i(mImage.cols, mImage.rows), bIsReadingColorImage ? true : false);
        }

        videoWriter << mImage;

        // showing the source image and result image
        if (bIsShowingResult) {
            imshow("Source", mImage);
            cv::waitKey(1);
        }

        if (oReader.GetSequenceNumber() == iStopNumber) {
            break;
        }
    }
	
    return 0;
}