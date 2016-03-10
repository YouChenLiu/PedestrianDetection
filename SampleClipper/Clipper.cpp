#include "../myLibrary/myImageSequence/myImageSequence.h"
#include "../myLibrary/myLabel/myLabel.h"
#include "../myLibrary/tinyxml2/tinyxml2.h"
#include <opencv2/imgproc.hpp>
#include <ctime>

int main(void) {
    // size for normalize the clips
    const cv::Size2i NormalizedSize(64, 128);

    // the minimun size for random generate the negative samples
    const cv::Size2i MinSize(10, 20);

    // how many samples for a image
    const int iNegativeSamplesPerImage = 4;

    // the XML file path
    const std::string sXMLFileName = "C:/Users/youch/Desktop/Waiting/2015-1005_0900-0915_04/2015-1005_0900-0915_04.xml";

    // the index of slash in the XML file path
    const auto iLastSlash = sXMLFileName.rfind("/") + 1;

    // root path for the images folder
    const std::string sRootPath = sXMLFileName.substr(0, iLastSlash);

    // the first number of image sequence
    const int iFirstNum = 4163;
    
    // use ground-truth to clip the positive samples.

    std::srand(static_cast<unsigned int>(time(NULL)));
    
    // the xml file
    tinyxml2::XMLDocument oXML;
    
    auto Result = oXML.LoadFile(sXMLFileName.c_str());
    if (Result != tinyxml2::XMLError::XML_NO_ERROR) {
        std::cout << "XML File" << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    
    // sequence for reading
    myImageSequence oImageReader(sRootPath, "", "bmp", false);
    oImageReader.SetAttribute(myImageSequence::Attribute::FIRST_NUMBER, iFirstNum);
    oImageReader.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);

    // sequence for writing negative samples
    myImageSequence oNegativeWriter(sRootPath + "Negative/", "", "bmp");
    oNegativeWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);

    // sequence for writing out the positive samples
    myImageSequence oPositiveWriter(sRootPath + "Positive/", "", "bmp");
    oPositiveWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);

    // the pointer to data set element
    auto poDataSet = oXML.RootElement()->FirstChildElement("DataSet");
    if (poDataSet == nullptr) {
        std::cout << "Not Found Data Set Tag" << std::endl;
        exit(EXIT_FAILURE);
    }

    // header tag string
    auto sHeaderTag = myLabel::GetLabel(myLabel::Tags::HEADER);

    // pointer for header element
    auto poHeader = poDataSet->FirstChildElement(sHeaderTag.c_str());
    if (poHeader == nullptr) {
        std::cout << "Not Found Any Header Tag" << std::endl;
        exit(EXIT_FAILURE);
    }

    // the reading image
    cv::Mat mImage;
    while (oImageReader >> mImage) {
        // the sequence number for reading now
        int iFrameNumber = oImageReader.GetSequenceNumber();
        
        // frame number string
        auto sFrameNumTag = myLabel::GetLabel(myLabel::Attributes::FRAME_NUMBER);

        // find the header tag which matching the iFrameNumber
        while ((poHeader != nullptr) &&
               (poHeader->IntAttribute(sFrameNumTag.c_str()) != iFrameNumber)) {
            poHeader = poHeader->NextSiblingElement();
        }

        // break when reach the file end
        if (poHeader == nullptr) {
            break;
        }
        
        // pointer to record tag
        auto poRecord = poHeader->FirstChildElement();
        while (poRecord != nullptr) {
            using std::string;

            // string for saving tag label
            string sTagStr = string();
            
            sTagStr = myLabel::GetLabel(myLabel::Tags::START_POINT);
            // start point string
            string sStart = poRecord->FirstChildElement(sTagStr.c_str())->GetText();
            
            sTagStr = myLabel::GetLabel(myLabel::Tags::HEIGHT);
            // height string
            string sHeight = poRecord->FirstChildElement(sTagStr.c_str())->GetText();
            
            sTagStr = myLabel::GetLabel(myLabel::Tags::WIDTH);
            // width string
            string sWidth = poRecord->FirstChildElement(sTagStr.c_str())->GetText();
            
            // stream for extracting value
            std::stringstream ss(sStart + "," + sHeight + "," + sWidth);
            char cComma;
            cv::Point2i StartPoint;
            cv::Size SampleSize;
            ss  >> StartPoint.x >> cComma >> StartPoint.y
                >> SampleSize.height >> cComma >> SampleSize.width;
            
            cv::Rect2i SampleRegion(StartPoint, SampleSize);
            cv::Mat mSubImage = mImage(SampleRegion);
            resize(mSubImage, mSubImage, NormalizedSize);
            oPositiveWriter << mSubImage;

            poRecord = poRecord->NextSiblingElement();
        }

        // use random select to generate negative samples
        for (int i = 0; i < iNegativeSamplesPerImage; ++i) {
            cv::Point2i RandomPoint(rand() % mImage.cols, rand() % mImage.rows);
            cv::Size2i RandomSize(rand() % NormalizedSize.width + MinSize.width,
                                  rand() % NormalizedSize.height + MinSize.height);
            
            if (RandomPoint.x + RandomSize.width >= mImage.cols) {
                RandomPoint.x = mImage.cols - 1 - RandomSize.width;
            }
            if (RandomPoint.y + RandomSize.height >= mImage.rows) {
                RandomPoint.y = mImage.rows - 1 - RandomSize.height;
            }
            cv::Rect2i RandomRegion(RandomPoint, RandomSize);
            cv::Mat mSubImage = mImage(RandomRegion);
            resize(mSubImage, mSubImage, NormalizedSize);
            oNegativeWriter << mSubImage;
        }
    }

    return 0;
}