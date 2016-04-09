/**
 * @file myLBP.h
 * @brief Local Binary Pattern class definition.
 */

#ifndef _MY_LBP_H_
#define _MY_LBP_H_

#include "../myBlockDescriptorBase.h"

namespace Descriptor {

  /**
   * @brief LBP feature extractor.
   */
  class myLBP : public myBlockDescriptorBase {
  protected:
    /// a flag means the feature is uniform or not.
    static const int UNIFORM_FLAG = 0x80 << PATTERN_OFFSET;
    /// the standard LBP feature.
    static const int LBP = WELL_KNOWN_FEATURE_FLAG | BLOCK_BASED_FLAG |
      (LBP_FEATURE << FEATURE_OFFSET);

  public:     // public attribute
      /**
       * @brief definition of difference LBP pattern.
       */
    class Feature : protected myFeatureBase {
    public:
      /// 8 points with radius 1.
      static const int LBP_8_1 = LBP | (0x00 << PATTERN_OFFSET);

      /// 8 points with radius 2.
      static const int LBP_8_2 = LBP | (0x01 << PATTERN_OFFSET);

      /// 16 points with radius 2.
      static const int LBP_16_2 = LBP | (0x02 << PATTERN_OFFSET);

      /// 8 points with radius 1 and uniform pattern.
      static const int LBP_8_1_UNIFORM = LBP_8_1 | UNIFORM_FLAG;

      /// 8 points with radius 2 and uniform pattern.
      static const int LBP_8_2_UNIFORM = LBP_8_2 | UNIFORM_FLAG;

      /// 16 points with radius 2 and uniform pattern.
      static const int LBP_16_2_UNIFORM = LBP_16_2 | UNIFORM_FLAG;
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
    static const unsigned int MAX_LENGTH = 16;

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

  private:    // private attribute
      /// Is this a uniform LBP?
    bool m_bIsUniform;

    /// Array for saving uniform bin number.
    static std::array<std::vector<bool>, myLBP::MAX_LENGTH / 8> m_avbUniformMap;

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

};
#endif