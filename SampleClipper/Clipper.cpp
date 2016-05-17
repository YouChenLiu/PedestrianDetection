#include "myImageSequence/myImageSequence.h"
#include "myPlugin/myLabel/myLabel.h"
#include "tinyxml2/tinyxml2.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <ctime>
#include <iomanip>

int main(void) {
  // XML file name
  const std::string sFolderName = "2015-1006_1335-1345_06";

  // root path for the images folder
  const std::string sRootPath = "C:/Users/youch/Desktop/Waiting/";

  // the XML file path
  const std::string sXMLFilePath = sRootPath + sFolderName + "/" +
    sFolderName + ".xml";

  // image file extension
  const std::string sExtension("bmp");

  // size for normalize the clips
  const cv::Size2i NormalizedSize(64, 128);

  // the minimun size for random generate the negative samples
  const cv::Size2i MinSize(10, 20);

  // how many samples for a image
  const int iNegativeSamplesPerImage = 4;

  const unsigned int iPaddingLength = 6;

  // root path for the images folder
  const std::string sImageRootPath = sRootPath + sFolderName + "/";

  std::srand(static_cast<unsigned int>(time(NULL)));

  // the xml file
  tinyxml2::XMLDocument oXML;

  auto Result = oXML.LoadFile(sXMLFilePath.c_str());
  if (Result != tinyxml2::XMLError::XML_NO_ERROR) {
    std::cout << "XML File" << std::endl;
    system("pause");
    exit(EXIT_FAILURE);
  }

  // sequence for writing negative samples
  myImageSequence oNegativeWriter(sImageRootPath + "Negative/", "", "bmp");
  oNegativeWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);

  // sequence for writing out the positive samples
  myImageSequence oPositiveWriter(sImageRootPath + "Positive/", "", "bmp");
  oPositiveWriter.SetAttribute(myImageSequence::Attribute::PADDING_LENGTH, 6);

  // the pointer to data set element
  auto poDataSet = oXML.RootElement()->FirstChildElement("DataSet");
  if (poDataSet == nullptr) {
    std::cout << "Not Found Data Set Tag" << std::endl;
    exit(EXIT_FAILURE);
  }

  using namespace Plugin;
  // header tag string
  auto sHeaderTag = GetLabel(Tags::HEADER);

  // pointer for header element
  auto poHeader = poDataSet->FirstChildElement(sHeaderTag.c_str());
  if (poHeader == nullptr) {
    std::cout << "Not Found Any Header Tag" << std::endl;
    exit(EXIT_FAILURE);
  }

  // create positive folder
  system(("mkdir \"" + sImageRootPath + "Positive\"").c_str());

  // create negative folder
  system(("mkdir \"" + sImageRootPath + "Negative\"").c_str());


  // frame number string
  auto sFrameNumTag = GetLabel(Attributes::FRAME_NUMBER);

  // find the header tag which matching the iFrameNumber
  while (poHeader != nullptr) {
    // the image sequence number
    auto iFrameNumber = poHeader->IntAttribute(sFrameNumTag.c_str());

    // image file path string stream
    std::stringstream ssPath;
    ssPath << sImageRootPath
      << std::setfill('0') << std::setw(iPaddingLength) << iFrameNumber
      << "." << sExtension;

    cv::Mat mImg = cv::imread(ssPath.str(), cv::IMREAD_GRAYSCALE);
    if (mImg.empty()) {
      std::cout << "Reading Image ERROR : " << iFrameNumber << std::endl;
      std::cout << ssPath.str() << std::endl;
      poHeader = poHeader->NextSiblingElement();
      continue;
    }

    // pointer to record tag
    auto poRecord = poHeader->FirstChildElement();
    while (poRecord != nullptr) {
      using std::string;

      // string for saving tag label
      string sLabel = string();

      // start point string
      sLabel = GetLabel(Tags::START_POINT);
      string sStart = poRecord->FirstChildElement(sLabel.c_str())->GetText();

      // height string
      sLabel = GetLabel(Tags::HEIGHT);
      string sHeight = poRecord->FirstChildElement(sLabel.c_str())->GetText();

      // width string
      sLabel = GetLabel(Tags::WIDTH);
      string sWidth = poRecord->FirstChildElement(sLabel.c_str())->GetText();

      // stream for extracting value
      std::stringstream ss(sStart + "," + sHeight + "," + sWidth);
      char cComma;
      cv::Point2i StartPoint;
      cv::Size SampleSize;
      ss >> StartPoint.x >> cComma >> StartPoint.y >> cComma
        >> SampleSize.height >> cComma >> SampleSize.width;

      if (StartPoint.x + SampleSize.width >= mImg.cols ||
          StartPoint.y + SampleSize.height >= mImg.rows) {
        poRecord = poRecord->NextSiblingElement();
        std::cout << "Sample out of range at " << iFrameNumber << std::endl;
        continue;
      }

      if (SampleSize.area() == 0) {
        poRecord = poRecord->NextSiblingElement();
        std::cout << "SampleSize.area() == 0 at " << iFrameNumber << std::endl;
        continue;
      }

      // use ground-truth to clip the positive samples.
      // clip the sample, resize to spesific size and write out

      cv::Rect2i SampleRegion(StartPoint, SampleSize);
      cv::Mat mSubImage = mImg(SampleRegion);
      resize(mSubImage, mSubImage, NormalizedSize);
      oPositiveWriter << mSubImage;

      // find the next record tag
      poRecord = poRecord->NextSiblingElement();
    }

    // use random select to generate negative samples
    for (int i = 0; i < iNegativeSamplesPerImage; ++i) {
      cv::Point2i RandomPoint(rand() % mImg.cols, rand() % mImg.rows);
      cv::Size2i RandomSize(rand() % NormalizedSize.width + MinSize.width,
                            rand() % NormalizedSize.height + MinSize.height);

      if (RandomPoint.x + RandomSize.width >= mImg.cols) {
        RandomPoint.x = mImg.cols - 1 - RandomSize.width;
      }
      if (RandomPoint.y + RandomSize.height >= mImg.rows) {
        RandomPoint.y = mImg.rows - 1 - RandomSize.height;
      }
      cv::Rect2i RandomRegion(RandomPoint, RandomSize);
      cv::Mat mSubImage = mImg(RandomRegion);
      resize(mSubImage, mSubImage, NormalizedSize);
      oNegativeWriter << mSubImage;
    }

    poHeader = poHeader->NextSiblingElement();
  }

  return 0;
}