#include "myBBReader.h"
#include <sstream>

namespace Plugin {
myBBReader::myBBReader(void) {
  Init();
}

myBBReader::myBBReader(const std::string & sFilePath) {
  Init();
  OpenFile(sFilePath);
  ScanFrames();
}

void myBBReader::LoadXML(const std::string & sPath) {
  OpenFile(sPath);
  ScanFrames();
}

void myBBReader::TakeOutBBbyFrame(int iFrameNum, std::vector<cv::Rect2i>& vrBB) const {
  auto element = m_mapElements.find(iFrameNum);
  if (element == m_mapElements.end()) {
    std::cout << "myBBReader::Frame Number not fount\t"
              << "number = " << iFrameNum << std::endl;
  } else {
    auto Header = element->second;
    auto Record = Header->FirstChildElement(GetLabel(Tags::RECORD).c_str());
    while (Record != nullptr) {
      auto r = ConvertToRect(Record);
      vrBB.push_back(r);
      Record = Record->NextSiblingElement();
    }
  }
}

bool myBBReader::IsEmpty(void) {
  return m_pXML == nullptr;
}

void myBBReader::Init(void) {
  m_pXML = nullptr;
  m_iFirstFrame = m_iEndFrame = -1;
}

void myBBReader::OpenFile(const std::string& sFilePath) {
  m_pXML = std::make_unique<tinyxml2::XMLDocument>();
  auto code = m_pXML->LoadFile(sFilePath.c_str());
  if (code != tinyxml2::XML_NO_ERROR) {
    m_pXML.release();
    m_pXML = nullptr;
    std::cout << "myBBReader::Open file ERROR!!\t"
              << "Error code = " << code
              << std::endl;
  }
}

void myBBReader::CloseFile(void) {}

void myBBReader::ScanFrames(void) {
  if (IsEmpty() == false) {
    m_mapElements.clear();
    auto Data = m_pXML->RootElement()->FirstChildElement(GetLabel(Tags::DATA_SET).c_str());
    auto Header = Data->FirstChildElement(GetLabel(Tags::HEADER).c_str());
    while (Header != nullptr) {
      auto iFrame = Header->IntAttribute(GetLabel(Attributes::FRAME_NUMBER).c_str());
      if (m_iFirstFrame == -1 && iFrame != 0) {
        m_iFirstFrame = iFrame;
      }
      m_mapElements.insert(std::pair<int, tinyxml2::XMLElement*>(iFrame, Header));
      Header = Header->NextSiblingElement();
      if (Header == nullptr) {
        m_iEndFrame = iFrame;
      }
    }
  }
}

cv::Rect2i myBBReader::ConvertToRect(tinyxml2::XMLElement* Record) const {
  //auto pcShape = Record->FirstChildElement(GetLabel(Tags::SHAPE).c_str())->GetText();
  std::string pcPoint = Record->FirstChildElement(GetLabel(Tags::START_POINT).c_str())->GetText();
  std::string pcHeight = Record->FirstChildElement(GetLabel(Tags::HEIGHT).c_str())->GetText();
  std::string pcWidth = Record->FirstChildElement(GetLabel(Tags::WIDTH).c_str())->GetText();
  std::stringstream ss(pcPoint + "," + pcHeight + "," + pcWidth);
  int x = 0, y = 0, width = 0, height = 0;
  char spilter = '\0';
  ss >> x >> spilter >> y >> spilter >> height >> spilter >> width;
  return cv::Rect2i(x, y, width, height);
}

} // namespace Plugin