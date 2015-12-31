#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/tinyxml2/tinyxml2.h"

const cv::Size2i NormalizedSize = { 64, 128 };
const cv::Size2i MinSize = { 10, 20 };
const int iNegativeSamplesPerImage = 4;

const std::string sRootPath = "D:/Database/spilt/2015-1005_2120-2130/04/";
const int iFirstNum = 4163;

int main(void) {
    using namespace std;
    using namespace cv;
    using namespace tinyxml2;

    /*
    ** use ground-truth to clip the positive samples.
    */
    
    tinyxml2::XMLDocument oXML;
    int i = static_cast<int>(sRootPath.length());
    string sXMLFileName = sRootPath + sRootPath.substr(sRootPath.length() - 3, 2) + ".xml";
    if (oXML.LoadFile(sXMLFileName.c_str()) != XMLError::XML_NO_ERROR) {
        cout << "XML File" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    
    myImageSequence oImageReader(sRootPath, "", "bmp", false);
    oImageReader.SetAttribute(myImageSequence::Attribute::FIRST_NUMBER, iFirstNum);
    oImageReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);

    myImageSequence oPositiveWriter((sRootPath + string("Positive/").c_str()), "", "bmp");
    oPositiveWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);

    Mat mImage;
    XMLElement* poDataSetElement = oXML.RootElement()->FirstChildElement("DataSet");
    if (poDataSetElement == nullptr) {
        exit(EXIT_FAILURE);
    }
    
    while (oImageReader >> mImage) {
        int iCurrentFrameNumber = oImageReader.GetSequenceNumber();
        
        XMLElement* poHeaderElement = poDataSetElement->FirstChildElement("Header");
        while ((poHeaderElement != nullptr) && (poHeaderElement->IntAttribute("frameNumber") != iCurrentFrameNumber)) {
            poHeaderElement = poHeaderElement->NextSiblingElement();
        }
        if (poHeaderElement == nullptr) {
            cout << "Frame " << iCurrentFrameNumber << " Not Found" << endl;
            break;
        }

        XMLElement* poRecordElement = poHeaderElement->FirstChildElement();
        while (poRecordElement != nullptr) {
            string sStartPoint = poRecordElement->FirstChildElement("StartPoint")->GetText();
            string sHeight = poRecordElement->FirstChildElement("Height")->GetText();
            string sWidth = poRecordElement->FirstChildElement("Width")->GetText();
        
            stringstream ssStartPoint(sStartPoint);
            Point2i StartPoint = { 0, 0 };
            char cComma;
            ssStartPoint >> StartPoint.x >> cComma >> StartPoint.y;
            int iHeight = stoi(sHeight);
            int iWidth = stoi(sWidth);

            Mat mSubImage = mImage.rowRange(StartPoint.y, StartPoint.y + iHeight).colRange(StartPoint.x, StartPoint.x + iWidth);
            resize(mSubImage, mSubImage, NormalizedSize);
            oPositiveWriter << mSubImage;

            poRecordElement = poRecordElement->NextSiblingElement();
        }
    }
    
    /*
    ** use random select to generate negative samples
    */

    oImageReader.SetAttribute(myImageSequence::Attribute::OFFSET, 0);
    myImageSequence oNegativeWriter((sRootPath + string("Negative/")).c_str(), "", "bmp");
    oNegativeWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);
    std::srand(static_cast<int>(time(NULL)));
    while (oImageReader >> mImage) {
        for (int i = 0; i < iNegativeSamplesPerImage; ++i) {
            Point2i RandomPoint(rand() % mImage.cols, rand() % mImage.rows);
            Size2i RandomSize(rand() % NormalizedSize.width + MinSize.width, rand() % NormalizedSize.height + MinSize.height);
            if (RandomPoint.x + RandomSize.width >= mImage.cols) {
                RandomPoint.x = mImage.cols - 1 - RandomSize.width;
            }
            if (RandomPoint.y + RandomSize.height >= mImage.rows) {
                RandomPoint.y = mImage.rows - 1 - RandomSize.height;
            }

            Mat mSubImage = mImage.rowRange(RandomPoint.y, RandomPoint.y + RandomSize.height).colRange(RandomPoint.x, RandomPoint.x + RandomSize.width);
            resize(mSubImage, mSubImage, NormalizedSize);
            oNegativeWriter << mSubImage;
        }
    }

    return 0;
}