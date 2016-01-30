#ifndef _MY_MODEL_COLLECTOR_H_
#define _MY_MODEL_COLLECTOR_H_

#include "../common.h"
#include "myModelCollectorBase.h"
#include <opencv2/ml.hpp>

class myModelCollector {
public:

    

public:
    myModelCollector(void);
    myModelCollector(unsigned int iNumOfModel);
    virtual ~myModelCollector(void);

    void TrainModels(void);
    void SaveModels(void);
    void LoadModels(std::string sFilePath);

private:
    bool m_bCreatingFromFile;
    std::vector<cv::Ptr<cv::ml::SVM>> m_vpoSVM;               // array for SVM pointers
    std::vector<std::vector<float>> m_vvvfFeature;



private:
    void Init(void);

};



#endif // !_MY_MODEL_COLLECTOR_H_

