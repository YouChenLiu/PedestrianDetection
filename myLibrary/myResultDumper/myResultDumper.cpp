#include "myResultDumper.h"

myResultDumper::myResultDumper(void) {
    Init();
    m_poXMLDocument = std::make_unique<tinyxml2::XMLDocument>();
    m_poXMLDocument->SetBOM(true);
    m_poXMLDocument->InsertFirstChild(m_poXMLDocument->NewDeclaration());
    tinyxml2::XMLElement* poRootElement = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::ROOT).c_str());
    tinyxml2::XMLElement* poDataSetElement = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::DATA_SET).c_str());
    poRootElement->InsertEndChild(poDataSetElement);
    m_poXMLDocument->InsertEndChild(poRootElement);
    AddNewHeader();
}

myResultDumper::~myResultDumper(void) {}

void myResultDumper::Init(void) {
    m_iFrameNumber = -1;
    m_iTotalRecord = 0;
    m_poCurrentHeader = nullptr;
}

void myResultDumper::AddNewHeader(void) {
    ++m_iFrameNumber;
    m_iTotalRecord = 0;
    
    tinyxml2::XMLElement* poHeader = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::HEADER).c_str());
    poHeader->SetAttribute(myXMLLabel::GetAttributeString(myXMLLabel::Attributes::FRAME_NUMBER).c_str(), m_iFrameNumber);
    poHeader->SetAttribute(myXMLLabel::GetAttributeString(myXMLLabel::Attributes::TOTAL_RECORD).c_str(), m_iTotalRecord);

    tinyxml2::XMLElement* poDataSetElement = m_poXMLDocument->RootElement()->FirstChildElement(myXMLLabel::GetTagString(myXMLLabel::Tags::DATA_SET).c_str());
    poDataSetElement->InsertEndChild(poHeader);

    m_poCurrentHeader = poHeader;
}

inline
void myResultDumper::AddNewRecord(myXMLLabel::Shapes shape, int x, int y, int iWidth, int iHeight) {
    tinyxml2::XMLElement* poRecord = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::RECORD).c_str());
    poRecord->SetAttribute(myXMLLabel::GetAttributeString(myXMLLabel::Attributes::SERIAL_NUMBER).c_str(), m_iTotalRecord);

    tinyxml2::XMLElement* poShape = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::SHAPE).c_str());
    poShape->SetText(myXMLLabel::GetShapeString(shape).c_str());
    poRecord->InsertEndChild(poShape);

    tinyxml2::XMLElement* poStartPoint = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::START_POINT).c_str());
    poStartPoint->SetText((std::to_string(x) + ", " + std::to_string(y)).c_str());
    poRecord->InsertEndChild(poStartPoint);

    tinyxml2::XMLElement* poHeight = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::HEIGHT).c_str());
    poHeight->SetText(std::to_string(iHeight).c_str());
    poRecord->InsertEndChild(poHeight);

    tinyxml2::XMLElement* poWidth = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::WIDTH).c_str());
    poWidth->SetText(std::to_string(iWidth).c_str());
    poRecord->InsertEndChild(poWidth);

    m_poCurrentHeader->InsertEndChild(poRecord);
    m_poCurrentHeader->SetAttribute(myXMLLabel::GetAttributeString(myXMLLabel::Attributes::TOTAL_RECORD).c_str(), ++m_iTotalRecord);
}

void myResultDumper::AddRectangle(int x, int y, int iWidth, int iHeight) {
    AddNewRecord(myXMLLabel::Shapes::RECTANGLE, x, y, iWidth, iHeight);
}

void myResultDumper::AddEllipse(int x, int y, int iMajorAxis, int iMinorAxis) {
    AddNewRecord(myXMLLabel::Shapes::ELLIPSE, x, y, iMajorAxis, iMinorAxis);
}

void myResultDumper::AddCircle(int x, int y, int iRadius) {
    AddNewRecord(myXMLLabel::Shapes::CIRCLE, x, y, iRadius, iRadius);
}

void myResultDumper::GoNextFrame(void) {
    AddNewHeader();
}

void myResultDumper::Save(std::string sFileName) {
    m_poXMLDocument->SaveFile(sFileName.c_str());
}