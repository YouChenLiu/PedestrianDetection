#ifndef _MY_MODEL_COLLECTOR_H_
#define _MY_MODEL_COLLECTOR_H_

#include "../common.h"
#include "myModelCollectorBase.h"
#include "../myClassifier/mySVM/mySVM.h"
#include "../myFeatureExtractor/myFeatureExtractor.h"

class myModelCollector final : public myModelCollectorBase {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
    // boolean for saving the collector is created from file or not
    bool m_bCreatingFromFile;

    // array for saving models such as svm
    std::vector<std::unique_ptr<mySVM>> m_vpoModel;

    std::unique_ptr<myFeatureExtractor> m_poExtractor;
    cv::Mat m_mImage;

public:     // public method
    myModelCollector(void);
    myModelCollector(unsigned int iNumOfModel);
    myModelCollector(const cv::Mat& mImage,
                     std::unique_ptr<myModelIndexerBase> poIndexer);
    virtual ~myModelCollector(void);

    void TrainModels(void);
    void SaveModels(void);
    void LoadModels(std::string sFilePath);
    void AddSample(cv::Point2i ptPosition, int iLabel);
    myExtractorBase* GetExtractor(void) {
        return m_poExtractor.get();
    }

protected:  // protected method

private:    // private method
    void Init(void);
    void AddSample(unsigned int iBinNumber, int iLabel,
                   const std::vector<float>& vfFeature);
};



#endif // !_MY_MODEL_COLLECTOR_H_

