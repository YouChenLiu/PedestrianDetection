#ifndef _MY_LBP_H_
#define _MY_LBP_H_

#include "../myBlockBasedExtractor/myBlockBasedExtractor.h"

/**
 * @brief LBP feature extractor.
 */
class myLBP : public myBlockBasedExtractor {
public:     // public attribute
    /**
     * @brief definition of difference LBP pattern.
     */
    class Feature : protected myFeatureBase {
    public:
        /// 8 points with radius 1.
        static const int LBP_8_1            = 0;

        /// 8 points with radius 2.
        static const int LBP_8_2            = 1;

        /// 16 points with radius 2.
        static const int LBP_16_2           = 2;

        /// 8 points with radius 1 and uniform pattern.
        static const int LBP_8_1_UNIFORM    = 3;

        /// 8 points with radius 2 and uniform pattern.
        static const int LBP_8_2_UNIFORM    = 4;

        /// 16 points with radius 2 and uniform pattern.
        static const int LBP_16_2_UNIFORM   = 5;
    };
    
    /// Number of patterns, not include uniform pattern
    static const unsigned int NUMBER_OF_PATTERNS = 3;
    
    /**
     * @brief Upper bound for trasition time
     *
     * The LBP feature wiil be nonuniform
     * if transition times (0 -> 1 or 1 -> 0) over it.
     */
    static const unsigned int MAX_TRANSITION_TIME = 2;

    /// Upper bound of LBP feature length
    static const unsigned int MAX_BIT_LENGTH = 16;
	
protected:  // protected attrribute
    /**
     * @brief Sampling points for LBP.
     * 
     * The sampling position around the center point for computing LBP feature.
     */
    static std::array<std::vector<cv::Point2i>,
                      myLBP::NUMBER_OF_PATTERNS> m_SamplingPoints;

    /// LBP radius, 1 or 2.
    unsigned int m_iRadius;

    /// LBP bit length, 8 or 16.
    unsigned int m_iLength;

    /// LBP pattern
    unsigned int m_iPattern;

private:    // private attribute
    /// Is this a uniform LBP?
    bool m_bIsUniform;

    /// Array for saving uniform bin number.
    static std::array<std::vector<bool>,
                      myLBP::MAX_BIT_LENGTH / 8> m_avbUniformMap;

public:     // public method
    /**
     * @brief Default Constructor.
     */
    myLBP(void);

    /**
     * @brief Recomend Constructor.
     * @param mImage A image want to compute feature.
     * @param Pattern The LBP Pattern.
     * @param BlockSize The region size for extractor feature.
     */
    myLBP(const cv::Mat& mImage, int Pattern,
          cv::Size2i BlockSize = cv::Size2i(8, 8));
    virtual ~myLBP(void);

    virtual void Describe(cv::Point2i Position,
                          std::vector<float>& vfFeature) const override;

#ifndef NDEBUG
    /**
     * @brief Print the LBP uniform map.
     * 
     * This function just exist with debug enviroment.
     * @param iLength The LBP bit length.
     */
    void PrintUniformMap(int iLength) const;
#endif

protected:  // protected method
    /**
     * @brief Compute the LBP index.
     * 
     * @param Position The left-top point for calculate feature.
     * @return Bin index.
     * @retval NAN The error occur when compute feature.
     */
    virtual unsigned int GetBinNumber(cv::Point2i Position) const;

    /**
     * @brief Compute the LBP index.
     *
     * @param mImg The 3 x 3 image for calculate feature.
     * @return Bin index.
     * @retval NAN The error occur when compute feature.
     */
    virtual unsigned int GetBinNumber(const cv::Mat& mImg) const;

    /**
     * @brief Check the number is uniform or not.
     * 
     * @param iPattern The used LBP pattern.
     * @param iBinNumber The number want to check.
     */
    static bool IsUniform(int iPattern, unsigned int iBinNumber);

    /**
     * @brief Check the number is uniform or not.
     *
     * @param iBinNumber The number want to check.
     * @param iLength The LBP bit length.
     */
    static bool IsUniform(unsigned int iBinNumber, unsigned int iLength);
private:    // private method
    void Init();
    void SetAttributes(int iPattern);
    static void SetSamplingPoints(void);
    
};

#endif