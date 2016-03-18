#include "myResultDumper.h"

myResultDumper::myResultDumper(void) {
    Init();
    m_poXMLDocument = std::make_unique<tinyxml2::XMLDocument>();
    m_poXMLDocument->SetBOM(true);
    m_poXMLDocument->InsertFirstChild(m_poXMLDocument->NewDeclaration());

    using std::string;
    string sRootTag = myLabel::GetLabel(myLabel::Tags::ROOT);
    auto poRootElement = m_poXMLDocument->NewElement(sRootTag.c_str());

    string sDataTag = myLabel::GetLabel(myLabel::Tags::DATA_SET);
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
    string sTag = myLabel::GetLabel(myLabel::Tags::HEADER);
    auto poHeader = m_poXMLDocument->NewElement(sTag.c_str());

    sTag = myLabel::GetLabel(
        myLabel::Attributes::FRAME_NUMBER);
    poHeader->SetAttribute(sTag.c_str(), m_iFrameNumber);

    sTag = myLabel::GetLabel(myLabel::Attributes::TOTAL_RECORD);
    poHeader->SetAttribute(sTag.c_str(), m_iTotalRecord);

    sTag = myLabel::GetLabel(myLabel::Tags::DATA_SET);
    auto poDataSetElement = m_poXMLDocument->RootElement()->FirstChildElement(sTag.c_str());
    poDataSetElement->InsertEndChild(poHeader);

    m_poCurrentHeader = poHeader;
}

inline
void myResultDumper::AddNewRecord(myLabel::Shapes shape, int x, int y, int iWidth, int iHeight) {
    auto poRecord = m_poXMLDocument->NewElement(myLabel::GetLabel(myLabel::Tags::RECORD).c_str());
    poRecord->SetAttribute(myLabel::GetLabel(myLabel::Attributes::SERIAL_NUMBER).c_str(),
        m_iTotalRecord);

    auto poShape = m_poXMLDocument->NewElement(myLabel::GetLabel(myLabel::Tags::SHAPE).c_str());
    poShape->SetText(myLabel::GetLabel(shape).c_str());
    poRecord->InsertEndChild(poShape);

    auto poStartPoint = m_poXMLDocument->NewElement(myLabel::GetLabel(myLabel::Tags::START_POINT).c_str());
    poStartPoint->SetText((std::to_string(x) + ", " + std::to_string(y)).c_str());
    poRecord->InsertEndChild(poStartPoint);

    auto poHeight = m_poXMLDocument->NewElement(myLabel::GetLabel(myLabel::Tags::HEIGHT).c_str());
    poHeight->SetText(std::to_string(iHeight).c_str());
    poRecord->InsertEndChild(poHeight);

    auto poWidth = m_poXMLDocument->NewElement(myLabel::GetLabel(myLabel::Tags::WIDTH).c_str());
    poWidth->SetText(std::to_string(iWidth).c_str());
    poRecord->InsertEndChild(poWidth);

    m_poCurrentHeader->InsertEndChild(poRecord);
    m_poCurrentHeader->SetAttribute(myLabel::GetLabel(myLabel::Attributes::TOTAL_RECORD).c_str(), ++m_iTotalRecord);
}

void myResultDumper::GoNextFrame(void) {
    AddNewHeader();
}

void myResultDumper::Save(const std::string& sFileName) const {
    m_poXMLDocument->SaveFile(sFileName.c_str());
}