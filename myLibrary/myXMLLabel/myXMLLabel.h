#ifndef _MY_XML_LABEL_H_
#define _MY_XML_LABEL_H_

#include <iostream>
#include <map>

class myXMLLabel {
public:
    enum class Tags {
        ROOT = 0,
        FILE_DESCRIPTION,
        VIDEO_FILE_NAME,
        CREATE_DATE,
        LASTEST_MODIFY_DATE,
        DATA_SET,
        HEADER,
        RECORD,
        SHAPE,
        START_POINT,
        HEIGHT,
        WIDTH
    };

    enum class Attributes {
        FRAME_NUMBER = 0,
        TOTAL_RECORD,
        SERIAL_NUMBER
    };

    enum class Shapes {
        RECTANGLE = 0,
        ELLIPSE,
        CIRCLE
    };

    static std::map<myXMLLabel::Tags, std::string> m_sTagStrings;
    static std::map<myXMLLabel::Shapes, std::string> m_sShapeStrings;
    static std::map<myXMLLabel::Attributes, std::string> m_sAttributeStrings;

public:
    myXMLLabel(void) {}
    static std::string GetTagString(Tags Label) { return m_sTagStrings[Label]; }
    static std::string GetShapeString(Shapes Shape) { return m_sShapeStrings[Shape]; }
    static std::string GetAttributeString(Attributes Attribute) { return m_sAttributeStrings[Attribute]; }
};

#endif