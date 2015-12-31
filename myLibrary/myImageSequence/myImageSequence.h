#ifndef _MY_IMAGE_SEQUENCE_H_
#define _MY_IMAGE_SEQUENCE_H_

#include <iostream>
#include <iomanip>
#include <opencv2/highgui.hpp>

// myImageSequence is a sequence images reader/writer.
// It can help you access image sequence by setting few parameters.
// You can adjust the parameters by setAttribute method
// if the default values are not meet your environment.
class myImageSequence {
public:
    enum class Attribute {
        ROOT_PATH = 0,
        PREFIX,
        EXTENSION,
        FIRST_NUMBER,
        PADDING_CHARACTER,
        PADDING_LENGTH,
        OFFSET,
        IS_COLOR
    };

private:
    std::string m_sRootPath;
    std::string m_sPrefix;
    std::string m_sExtension;
    int m_iFirstNumber;
	int m_iOffest;
    int m_iPaddingLength;
    char m_cPaddingCharacter;
    bool m_bIsColor;

public:
    // Default constructor
    // It usually needs to set parameters after creation.
    myImageSequence(void) { Init(); }
    
	// Recommend constructor
	// Setting the root path, prefix, file name extension and isColor.
    // EX : myImageSequence("C:/Images/", "BG-", "jpg", false).
    // myImageSequence will read/write C:\Images\0000.jpg, 0001.jpg, etc.
    // NOTE : The isColor parameter only effects when reading.
    myImageSequence(const std::string& sRootPath, const std::string& sPrefix = "", const std::string& sExtension = "bmp", bool bIsColor = true);
    
    ~myImageSequence(void) {}
    
    // Reading a image by myImageSequence
    // It will return true if success.
	bool ReadImage(cv::Mat& mImage);
    
    // Reading a image by myImageSequence
    // It will read a image as R value.
    // It's convenience when declaration a Mat.
    // Note : It will "not" tell you the reading operation is success or not.
    cv::Mat ReadImage(void);
    
    // Reading a image by myImageSequence
    // Let you access images like standard input cin.
    // It will return true if success.
    bool operator>>(cv::Mat& mImage);
    //friend bool operator>>(myImageSequence& lhs, cv::Mat& mImage) { return lhs.operator>>(mImage); }
    
    // Write out a image by myImageSequence
    // It will write image as a image sequence by parameters you set.
    // It will return true if success.
    bool WriteImage(const cv::Mat& mImage);
    
    // Write out a image by myImageSequence
    // Let you save images like standard output cout.
    // It will return true if success.
    bool operator<<(const cv::Mat& mImage);
    // friend bool operator<<(myImageSequence lhs, const cv::Mat& mImage) { return lhs.operator<<(mImage); }
    
    // This method provides the ability that can modify the parameter when running
    template <typename T>
    void SetAttribute(const Attribute attrbute, T value);
    
    // Return a integer which is the current processing number of myImageSequence.
    int GetSequenceNumber(void) const { return m_iFirstNumber + m_iOffest; }
    
    // Return a string which is the current processing number of myImageSequence.
    // like "0123" if padding length is 4.
    std::string GetSequenceNumberString(void) const;

private:
    void Init(void);
    std::string MakePath(void);
};

#endif