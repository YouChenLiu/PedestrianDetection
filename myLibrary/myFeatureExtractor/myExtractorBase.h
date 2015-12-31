#ifndef _MY_EXTRACTOR_BASE_H_
#define _MY_EXTRACTOR_BASE_H_

#include <opencv2/core.hpp>

class myExtractorBase {
    public:
        class myFeatureBase {
        public:
            static const int NONE = -1;
        };

        virtual void Describe(cv::Point2i, std::vector<float>&) const = 0;
};

#endif