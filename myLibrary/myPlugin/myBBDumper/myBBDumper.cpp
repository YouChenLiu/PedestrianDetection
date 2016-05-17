#include "myBBDumper.h"

namespace Plugin {

myDumper::myDumper(void) {
  Init();
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
  AddNewHeader();
}

myDumper::~myDumper(void) {}

void myDumper::Init(void) {
  m_iFrameNumber = -1;
  m_iTotalRecord = 0;
  m_poCurrentHeader = nullptr;
}

void myDumper::AddNewHeader(void) {
  ++m_iFrameNumber;
  m_iTotalRecord = 0;
  using namespace Plugin;
  auto pcTag = GetLabel(Tags::HEADER).c_str();
  auto poHeader = m_poXMLDocument->NewElement(pcTag);

  pcTag = GetLabel(Attributes::FRAME_NUMBER).c_str();
  poHeader->SetAttribute(pcTag, m_iFrameNumber);

  pcTag = GetLabel(Attributes::TOTAL_RECORD).c_str();
  poHeader->SetAttribute(pcTag, m_iTotalRecord);

  pcTag = GetLabel(Tags::DATA_SET).c_str();
  auto poDataSetElement = m_poXMLDocument->RootElement()->FirstChildElement(pcTag);
  poDataSetElement->InsertEndChild(poHeader);

  m_poCurrentHeader = poHeader;
}

void myDumper::AddNewRecord(Plugin::Shapes shape, int x, int y, int iWidth, int iHeight) {
  using namespace Plugin;
  auto poRecord = m_poXMLDocument->NewElement(GetLabel(Tags::RECORD).c_str());
  poRecord->SetAttribute(GetLabel(Attributes::SERIAL_NUMBER).c_str(),
                         m_iTotalRecord);

  auto poShape = m_poXMLDocument->NewElement(GetLabel(Tags::SHAPE).c_str());
  poShape->SetText(GetLabel(shape).c_str());
  poRecord->InsertEndChild(poShape);

  auto poStartPoint = m_poXMLDocument->NewElement(GetLabel(Tags::START_POINT).c_str());
  poStartPoint->SetText((std::to_string(x) + ", " + std::to_string(y)).c_str());
  poRecord->InsertEndChild(poStartPoint);

  auto poHeight = m_poXMLDocument->NewElement(GetLabel(Tags::HEIGHT).c_str());
  poHeight->SetText(std::to_string(iHeight).c_str());
  poRecord->InsertEndChild(poHeight);

  auto poWidth = m_poXMLDocument->NewElement(GetLabel(Tags::WIDTH).c_str());
  poWidth->SetText(std::to_string(iWidth).c_str());
  poRecord->InsertEndChild(poWidth);

  m_poCurrentHeader->InsertEndChild(poRecord);
  m_poCurrentHeader->SetAttribute(GetLabel(Attributes::TOTAL_RECORD).c_str(), ++m_iTotalRecord);
}

void myDumper::GoNextFrame(void) {
  AddNewHeader();
}

void myDumper::Save(const std::string& sFileName) const {
  m_poXMLDocument->SaveFile(sFileName.c_str());
}

} // namespace