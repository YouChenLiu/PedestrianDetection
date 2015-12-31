#include "myXMLLabel.h"

std::map<myXMLLabel::Tags, std::string> myXMLLabel::m_sTagStrings = {
    { myXMLLabel::Tags::ROOT, "Root" },
    { myXMLLabel::Tags::FILE_DESCRIPTION, "FileDescription" },
    { myXMLLabel::Tags::CREATE_DATE, "CreateDate" },
    { myXMLLabel::Tags::VIDEO_FILE_NAME, "VideoFileName" },
    { myXMLLabel::Tags::LASTEST_MODIFY_DATE, "LatestModifyDate" },
    { myXMLLabel::Tags::DATA_SET, "DataSet" },
    { myXMLLabel::Tags::HEADER, "Header" },
    { myXMLLabel::Tags::RECORD, "Record" },
    { myXMLLabel::Tags::SHAPE, "Shape" },
    { myXMLLabel::Tags::START_POINT, "StartPoint" },
    { myXMLLabel::Tags::HEIGHT, "Height" },
    { myXMLLabel::Tags::WIDTH, "Width" } };

std::map<myXMLLabel::Shapes, std::string> myXMLLabel::m_sShapeStrings = {
    { myXMLLabel::Shapes::RECTANGLE, "rectangle" },
    { myXMLLabel::Shapes::ELLIPSE, "ellipse" },
    { myXMLLabel::Shapes::CIRCLE, "circle" }
};

std::map<myXMLLabel::Attributes, std::string> myXMLLabel::m_sAttributeStrings = {
    { myXMLLabel::Attributes::FRAME_NUMBER, "frameNumber" },
    { myXMLLabel::Attributes::SERIAL_NUMBER, "serialNumber" },
    { myXMLLabel::Attributes::TOTAL_RECORD, "totalRecord" }
};
