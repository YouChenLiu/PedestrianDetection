/**
 * @file myLabel.h
 * @brief XML Label class definition.
 */

#ifndef _MY_LABEL_H_
#define _MY_LABEL_H_

#include "../common.h"
#include <map>

/**
 * @brief A lookup tabel for XML tags.
 */
class myLabel {
public:
    /**
     * @brief Pre-defined XML tags.
     */
    enum class Tags {
        ROOT,                   //!< Root tag
        FILE_DESCRIPTION,       //!< FileDescription tag
        VIDEO_FILE_NAME,        //!< VideoFile tag
        CREATE_DATE,            //!< CreateDate tag
        LASTEST_MODIFY_DATE,    //!< LastestModifyDate tag
        DATA_SET,               //!< DataSet tag
        HEADER,                 //!< Header tag
        RECORD,                 //!< Record tag
        SHAPE,                  //!< Shape tag
        START_POINT,            //!< StartPoint tag
        HEIGHT,                 //!< Height tag
        WIDTH                   //!< Width tag
    };

    /**
     * @brief Pre-defined XML attributes.
     */
    enum class Attributes {
        FRAME_NUMBER,           //!< frameNumber attribute
        TOTAL_RECORD,           //!< totalRecord attribute
        SERIAL_NUMBER           //!< serialNumber attribute
    };

    /**
     * @brief Pre-defined XML shape.
     */
    enum class Shapes {
        RECTANGLE,              //!< Rectangle
        ELLIPSE,                //!< Ellipse
        CIRCLE                  //!< Circle
    };

protected:  // protected attribute

private:    // private attribute
    /// Mapping Tags to string.
    static std::map<Tags, std::string> m_sTagStrings;

    /// Mapping Shapes to string.
    static std::map<Shapes, std::string> m_sShapeStrings;

    /// Mapping Attributes to string.
    static std::map<Attributes, std::string> m_sAttributeStrings;

public:     // public method
    /**
     * @brief Default constructor.
     */
    myLabel(void) {}

    /**
     * @brief Prototype for getting label
     *
     * @param Label The pre-define label for Attribute, Shape and Tag
     */
    template<typename T>
    static std::string GetLabel(T Label);

    /**
    * @brief Get attribute string
    *
    * @param Attribute Pre-defined Attributes
    */
    template<>
    static std::string GetLabel(myLabel::Attributes Attribute) {
        return m_sAttributeStrings[Attribute];
    }

    /**
    * @brief Get shape string
    *
    * @param Shape Pre-defined Shapes
    */
    template<>
    static std::string GetLabel(myLabel::Shapes Shape) {
        return m_sShapeStrings[Shape];
    }

    /**
     * @brief Get tag string
     *
     * @param Tag Pre-defined Tags
     */
    template<>
    static std::string GetLabel(myLabel::Tags Tag) {
        return m_sTagStrings[Tag];
    }
};

#endif