#ifndef _MY_RESULT_DUMPER_H_
#define _MY_RESULT_DUMPER_H_

#include "../common.h"
#include "../tinyxml2/tinyxml2.h"
#include "../myXMLLabel/myXMLLabel.h"
#include <memory>
#include <map>

/**
 * @brief write out the bounding box infomation to XML file.
 */
class myResultDumper {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
    std::unique_ptr<tinyxml2::XMLDocument> m_poXMLDocument;
    int m_iFrameNumber;
    int m_iTotalRecord;
    tinyxml2::XMLElement* m_poCurrentHeader;

public:     // public method
    /**
     * @brief Default constructor.
     */
    myResultDumper(void);
    ~myResultDumper(void);

    /**
     * @brief Add new feame in XML file.
     */
    void GoNextFrame(void);

    /**
     * @brief Add new rectangle to XML file.
     *
     * @param x X value of Left-top point.
     *
     * @param y Y value of Left-top point.
     * @param iWidth Rectangle width.
     * @param iHeight Rectangle height.
     */
    void AddRectangle(int x, int y, int iWidth, int iHeight) {
        AddNewRecord(myXMLLabel::Shapes::RECTANGLE, x, y, iWidth, iHeight);
    }
    
    /**
     * @brief Add new ellipse to XML file.
     *
     * @param x X value of Left-top point.
     * @param y Y value of Left-top point.
     * @param iMajorAxis Major axis of new ellipse.
     * @param iMinorAxis Minor axis of new ellipse.
     */
    void AddEllipse(int x, int y, int iMajorAxis, int iMinorAxis) {
        AddNewRecord(myXMLLabel::Shapes::ELLIPSE, x, y, iMajorAxis, iMinorAxis);
    }

    /**
     * @brief Add new circle to XML file.
     *
     * @param x X value of Left-top point.
     * @param y Y value of Left-top point.
     * @param iRadius Radius of new circle.
     */
    void AddCircle(int x, int y, int iRadius){
        AddNewRecord(myXMLLabel::Shapes::CIRCLE, x, y, iRadius, iRadius);
    }

    /**
    * @brief Save the XML file.
    *
    * @param sFileName File path for saving XML file.
    */
    void Save(const std::string& sFileName) const;

protected:  // protected method
    
private:    // private method
    void Init(void);
    void AddNewHeader(void);
    void AddNewRecord(myXMLLabel::Shapes Shape,
                      int x, int y, int iWidth, int iHeight);
};

#endif