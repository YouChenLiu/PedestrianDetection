#ifndef _MY_FEATURE_EXTRACTOR_H_
#define _MY_FEATURE_EXTRACTOR_H_

#include <memory>
#include "../myFeatureExtractor/myBlockBasedExtractor/myBlockBasedExtractor.h"
#include "myHOG/myHOG.h"
#include "myLBP/myLBP.h"

/**
 * @brief Class for computing any blockbased feature.
 */
class myFeatureExtractor : protected myBlockBasedExtractor {
public:
    /// The predefine pattern value.
    class Features : public myHOG::Feature, public myLBP::Feature {};

private:
    /// The vector of extractors.
    std::vector<std::unique_ptr<myExtractorBase>> m_vpoUsedExtractor;

public:
    /**
     * @brief Create with a image.
     *
     * @param mImage A image for computing feature.
     * @param BlockSize A region size for computing feature.
     */
    myFeatureExtractor(const cv::Mat& mImage, cv::Size2i BlockSize = cv::Size2i(8, 8));

    /**
     * @brief Create with a IplImage.
     *
     * @param pImage A image for computing feature.
     * @param BlockSize A region size for computing feature.
     */
    myFeatureExtractor(IplImage* pImage, CvSize BlockSize = cvSize(8, 8));
    ~myFeatureExtractor(void);

    void Describe(cv::Point2i Position, std::vector<float>& vfFeature) const;

    /**
     * @brief Add new extractor for extracting feature.
     *
     * @param iFeature The predefine pattern in Features
     */
    void EnableFeature(int iFeature);

    /**
     * @brief Set the block size.
     *
     * @param Size The new size.
     */
    void SetBlockSize(cv::Size2i Size) { m_BlockSize = Size; }

    /**
     * @brief Set the block size.
     *
     * @param Width The width in new blick size.
     * @param Height The height in new block size.
     */
    void SetBlockSize(int Width, int Height) { SetBlockSize(cv::Size2i(Width, Height)); }

private:
    void Init(void);
    
};

#endif