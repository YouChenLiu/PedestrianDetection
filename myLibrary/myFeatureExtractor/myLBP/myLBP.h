#ifndef _MY_LBP_H_
#define _MY_LBP_H_

#include "../myBlockBasedExtractor/myBlockBasedExtractor.h"

#ifndef NDEBUG
#	include <iostream>
#   include <iomanip>
#endif

class myLBP : public myBlockBasedExtractor {
public:     // public attribute
    class Feature : protected myFeatureBase {
    public:
        static const int LBP_8_1            = 0;
        static const int LBP_8_2            = 1;
        static const int LBP_16_2           = 2;
        static const int LBP_8_1_UNIFORM    = 3;
        static const int LBP_8_2_UNIFORM    = 4;
        static const int LBP_16_2_UNIFORM   = 5;
    };
    
    static const unsigned int NUMBER_OF_PATTERNS = 3;    // the number of patterns, not include uniform pattern
    static const unsigned int MAX_TRANSITION_TIME = 2;   // the LBP feature wiil be nonuniform if times oftransition (0 -> 1 or 1 -> 0) over it.
    static const unsigned int MAX_BIT_LENGTH = 16;       // upper bound of LBP feature length
	
protected:  // protected attrribute
    static std::array<std::vector<cv::Point2i>, myLBP::NUMBER_OF_PATTERNS> m_SamplingPoints;
    unsigned int m_iRadius;
    unsigned int m_iLength;
    unsigned int m_iPattern;

private:    // private attribute
    bool m_bIsUniform;
    static std::array<std::vector<bool>, myLBP::MAX_BIT_LENGTH / 8> m_avbUniformMap;

public:     // public method
    myLBP(void);
    myLBP(const cv::Mat& mImage, int Pattern,
          cv::Size2i blockSize = cv::Size2i(8, 8));
    virtual ~myLBP(void);

    virtual void Describe(cv::Point2i Position,
                          std::vector<float>& vfFeature) const override;

#ifndef NDEBUG
    void PrintUniformMap(int iLength) const;
#endif

protected:  // protected method
    virtual unsigned int GetBinNumber(cv::Point2i Position) const;
    virtual unsigned int GetBinNumber(const cv::Mat& mImg) const;
    static bool IsUniform(int iPattern, unsigned int iBinNumber);
    static bool IsUniform(unsigned int iBinNumber, unsigned int iLength);
private:    // private method
    void Init();
    void SetAttributes(int iPattern);
    static void SetSamplingPoints(void);
    
};

#endif