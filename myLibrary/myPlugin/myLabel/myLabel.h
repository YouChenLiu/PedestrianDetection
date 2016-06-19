/**
 * @file myLabel.h
 * @brief XML Label definition.
 *
 * Define labels and some function for accessing.
 */

#ifndef _MY_LABEL_H_
#define _MY_LABEL_H_

#include "common.h"
#include <map>

namespace Plugin {
/**
 * @brief Pre-defined XML tags.
 */
enum class Tags {
  ROOT,                   //!< Root tag
  FILE_DESC,              //!< FileDescription tag
  VIDEO_NAME,             //!< VideoFile tag
  CREATE_DATE,            //!< CreateDate tag
  MODIFY_DATE,            //!< LastestModifyDate tag
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

/**
 * @brief XML Tags
 */
static std::map<Tags, std::string> m_sTagStrings = {
    { Tags::ROOT,                   "Root" },
    { Tags::FILE_DESC,              "FileDescription" },
    { Tags::CREATE_DATE,            "CreateDate" },
    { Tags::VIDEO_NAME,             "VideoFileName" },
    { Tags::MODIFY_DATE,            "LatestModifyDate" },
    { Tags::DATA_SET,               "DataSet" },
    { Tags::HEADER,                 "Header" },
    { Tags::RECORD,                 "Record" },
    { Tags::SHAPE,                  "Shape" },
    { Tags::START_POINT,            "StartPoint" },
    { Tags::HEIGHT,                 "Height" },
    { Tags::WIDTH,                  "Width" }
};

/**
 * @brief XML shapes
 */
static std::map<Shapes, std::string> m_sShapeStrings = {
    { Shapes::RECTANGLE,        "rectangle" },
    { Shapes::ELLIPSE,          "ellipse" },
    { Shapes::CIRCLE,           "circle" }
};

/**
 * @brief XML attributes
 */
static std::map<Attributes, std::string> m_sAttributeStrings = {
    { Attributes::FRAME_NUMBER,     "frameNumber" },
    { Attributes::SERIAL_NUMBER,    "serialNumber" },
    { Attributes::TOTAL_RECORD,     "totalRecord" }
};

/**
 * @brief Get label string
 *
 * @param Label Pre-defined label
 */
template<typename T> inline
std::string GetLabel(T Label);

/**
 * @brief Get attribute string
 *
 * @param Attribute Pre-defined Attributes
 */
template<> inline
std::string GetLabel(Attributes Attribute) {
  return m_sAttributeStrings.at(Attribute);
}

/**
 * @brief Get shape string
 *
 * @param Shape Pre-defined Shapes
 */
template<> inline
std::string GetLabel(Shapes Shape) {
  return m_sShapeStrings.at(Shape);
}

/**
 * @brief Get tag string
 *
 * @param Tag Pre-defined Tags
 */
template<> inline
std::string GetLabel(Tags Tag) {
  return m_sTagStrings.at(Tag);
}

} // namespace Plugin
#endif