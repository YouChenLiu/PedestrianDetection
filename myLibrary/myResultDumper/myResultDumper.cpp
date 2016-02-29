#include "myResultDumper.h"

myResultDumper::myResultDumper(void) {
    Init();
    m_poXMLDocument = std::make_unique<tinyxml2::XMLDocument>();
    m_poXMLDocument->SetBOM(true);
    m_poXMLDocument->InsertFirstChild(m_poXMLDocument->NewDeclaration());

    using std::string;
    string sRootTag = myXMLLabel::GetTagString(myXMLLabel::Tags::ROOT);
    auto poRootElement = m_poXMLDocument->NewElement(sRootTag.c_str());

    string sDataTag = myXMLLabel::GetTagString(myXMLLabel::Tags::DATA_SET);
    auto poDataSetElement = m_poXMLDocument->NewElement(sDataTag.c_str());

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
    
    using std::string;
    string sTag = myXMLLabel::GetTagString(myXMLLabel::Tags::HEADER);
    auto poHeader = m_poXMLDocument->NewElement(sTag.c_str());

    sTag = myXMLLabel::GetAttributeString(
        myXMLLabel::Attributes::FRAME_NUMBER);
    poHeader->SetAttribute(sTag.c_str(), m_iFrameNumber);

    sTag = myXMLLabel::GetAttributeString(myXMLLabel::Attributes::TOTAL_RECORD);
    poHeader->SetAttribute(sTag.c_str(), m_iTotalRecord);

    sTag = myXMLLabel::GetTagString(myXMLLabel::Tags::DATA_SET);
    auto poDataSetElement = m_poXMLDocument->RootElement()->FirstChildElement(sTag.c_str());
    poDataSetElement->InsertEndChild(poHeader);

    m_poCurrentHeader = poHeader;
}

inline
void myResultDumper::AddNewRecord(myXMLLabel::Shapes shape, int x, int y, int iWidth, int iHeight) {
    auto poRecord = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::RECORD).c_str());
    poRecord->SetAttribute(myXMLLabel::GetAttributeString(myXMLLabel::Attributes::SERIAL_NUMBER).c_str(),
        m_iTotalRecord);

    auto poShape = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::SHAPE).c_str());
    poShape->SetText(myXMLLabel::GetShapeString(shape).c_str());
    poRecord->InsertEndChild(poShape);

    auto poStartPoint = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::START_POINT).c_str());
    poStartPoint->SetText((std::to_string(x) + ", " + std::to_string(y)).c_str());
    poRecord->InsertEndChild(poStartPoint);

    auto poHeight = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::HEIGHT).c_str());
    poHeight->SetText(std::to_string(iHeight).c_str());
    poRecord->InsertEndChild(poHeight);

    auto poWidth = m_poXMLDocument->NewElement(myXMLLabel::GetTagString(myXMLLabel::Tags::WIDTH).c_str());
    poWidth->SetText(std::to_string(iWidth).c_str());
    poRecord->InsertEndChild(poWidth);

    m_poCurrentHeader->InsertEndChild(poRecord);
    m_poCurrentHeader->SetAttribute(myXMLLabel::GetAttributeString(myXMLLabel::Attributes::TOTAL_RECORD).c_str(), ++m_iTotalRecord);
}

void myResultDumper::GoNextFrame(void) {
    AddNewHeader();
}

void myResultDumper::Save(const std::string& sFileName) const {
    m_poXMLDocument->SaveFile(sFileName.c_str());
}