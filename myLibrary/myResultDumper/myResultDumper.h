#ifndef _MY_RESULT_DUMPER_H_
#define _MY_RESULT_DUMPER_H_

#include <memory>
#include <map>
#include <vector>
#include <string>
#include "../tinyxml2/tinyxml2.h"
#include "../myXMLLabel/myXMLLabel.h"

class myResultDumper {
public:

public:
    myResultDumper(void);
    ~myResultDumper(void);

    void GoNextFrame(void);
    void AddRectangle(int x, int y, int iWidth, int iHeight);
    void AddEllipse(int x, int y, int iMajorAxis, int iMinorAxis);
    void AddCircle(int x, int y, int iRadius);
    void Save(std::string sFileName);
private:
    std::unique_ptr<tinyxml2::XMLDocument> m_poXMLDocument;
    int m_iFrameNumber;
    int m_iTotalRecord;
    tinyxml2::XMLElement* m_poCurrentHeader;
    
private:
    void Init(void);
    void AddNewHeader(void);
    void AddNewRecord(myXMLLabel::Shapes Shape, int x, int y, int iWidth, int iHeight);
    void InitDictionary(void);
};

#endif