#include "myBBDumper.h"
#include <sstream>

namespace Plugin {

myBBDumper::myBBDumper(void) {
  Init();
  CreateXMLFile();
}

myBBDumper::~myBBDumper(void) {
  m_poXMLDocument.release();
}

void myBBDumper::Init(void) {
  m_iTotalRecord = 0;
  m_poCurrentHeader = nullptr;
}

void myBBDumper::CreateXMLFile(void) {
  m_poXMLDocument = std::make_unique<tinyxml2::XMLDocument>();
  m_poXMLDocument->SetBOM(true);
  m_poXMLDocument->InsertFirstChild(m_poXMLDocument->NewDeclaration());
  using namespace Plugin;
  std::string sRootTag = GetLabel(Tags::ROOT);
  auto poRootElement = m_poXMLDocument->NewElement(sRootTag.c_str());

  std::string sDataTag = GetLabel(Tags::DATA_SET);
  auto poDataSetElement = m_poXMLDocument->NewElement(sDataTag.c_str());

  poRootElement->InsertEndChild(poDataSetElement);
  m_poXMLDocument->InsertEndChild(poRootElement);
}

void myBBDumper::AddNewHeader(int iFrameNum) {
  m_iTotalRecord = 0;
  auto psTag = GetLabel(Tags::HEADER);
  auto poHeader = m_poXMLDocument->NewElement(psTag.c_str());

  psTag = GetLabel(Attributes::FRAME_NUMBER);
  poHeader->SetAttribute(psTag.c_str(), iFrameNum);

  psTag = GetLabel(Attributes::TOTAL_RECORD);
  poHeader->SetAttribute(psTag.c_str(), m_iTotalRecord);

  psTag = GetLabel(Tags::DATA_SET);
  auto poDataSetElement = m_poXMLDocument->RootElement()->FirstChildElement(psTag.c_str());
  poDataSetElement->InsertEndChild(poHeader);
  
  m_poCurrentHeader = poHeader;
}

void myBBDumper::AddNewRecord(Shapes shape, int x, int y, int iWidth, int iHeight) {
  auto poRecord = m_poXMLDocument->NewElement(GetLabel(Tags::RECORD).c_str());
  poRecord->SetAttribute(GetLabel(Attributes::SERIAL_NUMBER).c_str(),
                         m_iTotalRecord);

  auto pcShape = m_poXMLDocument->NewElement(GetLabel(Tags::SHAPE).c_str());
  pcShape->SetText(GetLabel(shape).c_str());
  poRecord->InsertEndChild(pcShape);

  auto pcSP = m_poXMLDocument->NewElement(GetLabel(Tags::START_POINT).c_str());
  std::stringstream ss;
  ss << x << ", " << y;
  pcSP->SetText(ss.str().c_str());
  poRecord->InsertEndChild(pcSP);

  auto pcHeight = m_poXMLDocument->NewElement(GetLabel(Tags::HEIGHT).c_str());
  pcHeight->SetText(std::to_string(iHeight).c_str());
  poRecord->InsertEndChild(pcHeight);

  auto pcWidth = m_poXMLDocument->NewElement(GetLabel(Tags::WIDTH).c_str());
  pcWidth->SetText(std::to_string(iWidth).c_str());
  poRecord->InsertEndChild(pcWidth);

  m_poCurrentHeader->InsertEndChild(poRecord);
  m_poCurrentHeader->SetAttribute(GetLabel(Attributes::TOTAL_RECORD).c_str(),
                                  ++m_iTotalRecord);
}

void myBBDumper::AddFrame(int iFrameNum) {
  AddNewHeader(iFrameNum);
}

void myBBDumper::Save(const std::string& sFileName) const {
  m_poXMLDocument->SaveFile(sFileName.c_str());
}

} // namespace