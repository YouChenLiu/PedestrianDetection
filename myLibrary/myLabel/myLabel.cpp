#include "myLabel.h"

std::map<myLabel::Tags, std::string> myLabel::m_sTagStrings = {
    { Tags::ROOT,                   "Root" },
    { Tags::FILE_DESCRIPTION,       "FileDescription" },
    { Tags::CREATE_DATE,            "CreateDate" },
    { Tags::VIDEO_FILE_NAME,        "VideoFileName" },
    { Tags::LASTEST_MODIFY_DATE,    "LatestModifyDate" },
    { Tags::DATA_SET,               "DataSet" },
    { Tags::HEADER,                 "Header" },
    { Tags::RECORD,                 "Record" },
    { Tags::SHAPE,                  "Shape" },
    { Tags::START_POINT,            "StartPoint" },
    { Tags::HEIGHT,                 "Height" },
    { Tags::WIDTH,                  "Width" }
};

std::map<myLabel::Shapes, std::string> myLabel::m_sShapeStrings = {
    { Shapes::RECTANGLE,        "rectangle" },
    { Shapes::ELLIPSE,          "ellipse" },
    { Shapes::CIRCLE,           "circle" }
};

std::map<myLabel::Attributes, std::string> myLabel::m_sAttributeStrings = {
    { Attributes::FRAME_NUMBER,     "frameNumber" },
    { Attributes::SERIAL_NUMBER,    "serialNumber" },
    { Attributes::TOTAL_RECORD,     "totalRecord" }
};
