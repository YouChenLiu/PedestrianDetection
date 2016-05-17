/**
 * @file myBBReader.h read the bounding box from xml file
 */

#ifndef _MY_BB_READER_H_
#define _MY_BB_READER_H_

#include <memory>
#include <map>
#include "tinyxml2/tinyxml2.h"
#include "myPlugin/myLabel/myLabel.h"

namespace Plugin {

/**
 * @brief bounding boxes reader
 */
class myBBReader final {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
  std::unique_ptr<tinyxml2::XMLDocument> m_pXML;
  std::map<int, tinyxml2::XMLElement*> m_mapElements;
  int m_iFirstFrame;
  int m_iEndFrame;

public:     // public method
  /**
   * @brief default constructor
   */
  myBBReader(void);

  /**
   * @brief create object by a xml file path
   */
  myBBReader(const std::string& sFilePath);
  ~myBBReader() {}

  /**
   * @brief laod xml file
   */
  void LoadXML(const std::string& sPath);
  
  /**
   * @brief extract bounding boxes with sepsific frame number
   */
  void TakeOutBBbyFrame(int iFrameNum, std::vector<cv::Rect2i>& vrBB) const;

  /**
   * @brief check the file state
   *
   * @return the object mantains a file or not
   */
  bool IsEmpty(void);

  /**
   * @brief get the first frame number
   *
   * return the first frame number in xml file
   */
  auto GetFirstFrameNum(void) const { return m_iFirstFrame; }

  /**
   * @brief get the end frame number
   *
   * return the end frame number in xml file
   */
  auto GetEndFrameNum(void) const { return m_iEndFrame; }

protected:  // protected method

private:    // private method
  void Init(void);
  void OpenFile(const std::string& sFilePath);
  void CloseFile(void);
  void ScanFrames(void);
  cv::Rect2i ConvertToRect(tinyxml2::XMLElement* Record) const;
};

} // namespace Plugin

#endif // !_MY_BB_READER_H_