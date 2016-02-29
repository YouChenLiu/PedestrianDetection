#ifndef _MY_IMAGE_SEQUENCE_H_
#define _MY_IMAGE_SEQUENCE_H_

#include "../common.h"

/**
 * @brief sequence images reader/writer.
 *
 * myImageSequence is a sequence images reader/writer.
 * It can help you access image sequence by setting few parameters.
 * You can adjust the parameters by setAttribute method
 * if the default values are not meet your environment.
 */
class myImageSequence {
public:
    /**
     * @brief The attributes for setting object.
     */
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
    /**
     * @brief Default constructor.
     *
     * It needs to set parameters after creation.
     */
    myImageSequence(void) { Init(); }
    
	/**
     * @brief Recommend constructor.
     *
	 * Setting the root path, prefix, file name extension and isColor.
     *
     * EX : myImageSequence("C:/Images/", "BG-", "jpg", false).\n
     * myImageSequence will read/write C:/Images/0000.jpg, 0001.jpg, etc.
     *
     * @param sRootPath The root path for process images.
     * @param sPrefix String for prefix.
     * @param sExtension String for file extension.
     * @param bIsColor Determine the object read color(true) or
     * gray(false) image.
     * This parameter only effects when reading.
     */
    myImageSequence(const std::string& sRootPath,
                    const std::string& sPrefix = "",
                    const std::string& sExtension = "bmp",
                    bool bIsColor = true);
    
    ~myImageSequence(void) {}
    
    /**
     * @brief Read a image by myImageSequence.
     *
     * @param mImage A Mat store the read image by sequence.
     * @return Operation is successful or not.
     * @retval True read successful.
     * @retval False error occurd.
     */
	bool ReadImage(cv::Mat& mImage);
    
    /**
     * @brief Read a image by myImageSequence.
     *
     * It will read a image and return it as R value.
     * It's convenience when declaration a Mat.
     * Note : It will "not" tell you the reading operation is success or not.
     * @return A image in sequence.
     */
    cv::Mat ReadImage(void);
    
    /**
     * @brief Reading a image by myImageSequence.
     *
     * This function let you access images like standard input cin.
     * @param mImage A image want to write out by sequence.
     * @return Operation is successful or not.
     * @retval True Read successful.
     * @retval False Error occurd.
     */
    bool operator>>(cv::Mat& mImage);
    
    /**
     * @brief Write out a image by myImageSequence.
     *
     * @param mImage A image want to write out by sequence.
     * @return Operation is successful or not.
     * @retval True Write successful.
     * @retval False Error occurd.
     */
    bool WriteImage(const cv::Mat& mImage);
    
    /**
     * @brief Write out a image by myImageSequence.
     *
     * This function let you save images like standard output cout.
     * @param mImage A image want to write out by sequence.
     * @return Operation is successful or not.
     * @retval True Write successful.
     * @retval False Error occurd.
     */
    bool operator<<(const cv::Mat& mImage);
    
    /**
     * @brief The prototype for SetAttribute
     *
     * @param attribute The setting goal.
     * @param value A spesific value for attribute.
     */
    template <typename T>
    void SetAttribute(const Attribute attribute, T value);
    
    /**
     * @brief Return a integer
     * which is the current processing number of myImageSequence.
     */
    int GetSequenceNumber(void) const { return m_iFirstNumber + m_iOffest; }
    
    /**
     * @brief Return a string
     * which is the current processing number of myImageSequence.
     *
     * For example "0123" if padding length is 4.
     * @return sequence number string.
     */
    std::string GetSequenceNumberString(void) const;

private:
    void Init(void);
    std::string MakePath(void);
};

#endif