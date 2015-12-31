#include "myImageSequence.h"

myImageSequence::myImageSequence(const std::string& sRootPath, const std::string& sPrefix, const std::string& sExtension, bool bIsColor) {
    Init();
    m_sRootPath = sRootPath;
    m_sPrefix = sPrefix;
    m_sExtension = sExtension;
    m_bIsColor = bIsColor;
}

void myImageSequence::Init(void) {
    m_sRootPath = "";
    m_sPrefix = "";
    m_sExtension = "bmp";
	m_iOffest = -1;
    m_iFirstNumber = 0;
    m_iPaddingLength = 4;
    m_cPaddingCharacter = '0';
    m_bIsColor = true;
}

std::string myImageSequence::MakePath(void) {
    ++m_iOffest;
    return m_sRootPath + m_sPrefix + GetSequenceNumberString() + "." + m_sExtension;
}

bool myImageSequence::ReadImage(cv::Mat& mImage) {
    std::string sPath = MakePath();
    mImage = cv::imread(sPath, m_bIsColor ? cv::IMREAD_COLOR : cv::IMREAD_GRAYSCALE);
    return mImage.empty() ? false : true;
}

cv::Mat myImageSequence::ReadImage(void) {
    std::string sPath = MakePath();
    return cv::imread(sPath, m_bIsColor ? cv::IMREAD_COLOR : cv::IMREAD_GRAYSCALE);
}

bool myImageSequence::operator>>(cv::Mat& mImage) {
    std::string sPath = MakePath();
    mImage = cv::imread(sPath, m_bIsColor ? cv::IMREAD_COLOR : cv::IMREAD_GRAYSCALE);
    return mImage.empty() ? false : true;
}

bool myImageSequence::WriteImage(const cv::Mat& mImage) {
    std::string sPath = MakePath();
    return cv::imwrite(sPath, mImage);
}

bool myImageSequence::operator<<(const cv::Mat& mImage) {
    std::string sPath = MakePath();
    return cv::imwrite(sPath, mImage);
}

template <>
void myImageSequence::SetAttribute(const Attribute attrbute, const char* sValue) {
    switch (attrbute) {
    case Attribute::EXTENSION:
        m_sExtension = sValue;
        break;
    case Attribute::PREFIX:
        m_sPrefix = sValue;
        break;
    case Attribute::ROOT_PATH:
        m_sRootPath = sValue;
        break;
    default:
        std::cout << "ImageSequence::Error occur when setting string attribute" << std::endl;
        break;
    }
}

template <>
void myImageSequence::SetAttribute(const Attribute attrbute, int iValue) {
    switch (attrbute) {
    case Attribute::FIRST_NUMBER:
        m_iFirstNumber = iValue;
        break;
    case Attribute::PADDING_LENGTH:
        m_iPaddingLength = iValue;
        break;
    case Attribute::OFFSET:
        m_iOffest = iValue;
        break;
    default:
        std::cout << "ImageSequence::Error occur when setting integer attribute" << std::endl;
        break;
    }
}

template <>
void myImageSequence::SetAttribute(const Attribute attrbute, char cValue) {
    switch (attrbute) {
    case Attribute::PADDING_CHARACTER:
        m_cPaddingCharacter = cValue;
        break;
    default:
        std::cout << "ImageSequence::Error occur when setting character attribute" << std::endl;
        break;
    }
}

template <>
void myImageSequence::SetAttribute(const Attribute attrbute, bool bValue) {
    switch (attrbute) {
    case Attribute::IS_COLOR:
        m_bIsColor = bValue;
        break;
    default:
        std::cout << "ImageSequence::Error occur when setting boolean attribute" << std::endl;
        break;
    }
}

std::string myImageSequence::GetSequenceNumberString(void) const {
    std::stringstream sSequenceNumber;
    sSequenceNumber << std::setfill(m_cPaddingCharacter)
                    << std::setw(m_iPaddingLength)
					<< (m_iFirstNumber + m_iOffest);
    return sSequenceNumber.str();
}