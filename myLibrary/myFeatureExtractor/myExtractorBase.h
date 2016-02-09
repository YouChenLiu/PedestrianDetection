#ifndef _MY_EXTRACTOR_BASE_H_
#define _MY_EXTRACTOR_BASE_H_

#include "../common.h"

class myExtractorBase {
public:     // public attribute
    // define the different pattern for the derived extractor
    class myFeatureBase {
    public:
        static const int NONE = -1;
    };

protected:  // protected attribute
    // the image for extracting feature
    cv::Mat m_mImage;

private:    // private attribute

public:     // public method
    myExtractorBase(void);
    myExtractorBase(const cv::Mat& mImage);
    virtual ~myExtractorBase(void);

    // pure virtual function for describe the feature at sepecify position
    virtual void Describe(cv::Point2i, std::vector<float>&) const = 0;

    // setting the image want to extract feature
    void SetImage(const cv::Mat& mImage) { m_mImage = mImage; }

protected:  // protectd method

private:    // private method

};

#endif