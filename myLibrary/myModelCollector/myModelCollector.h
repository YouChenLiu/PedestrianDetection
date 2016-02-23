#ifndef _MY_MODEL_COLLECTOR_H_
#define _MY_MODEL_COLLECTOR_H_

#include "myModelCollectorBase.h"
#include "../common.h"
#include "../myClassifier/mySVM/mySVM.h"
#include <memory>

class myModelCollector final : public myModelCollectorBase {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
    // serial number
    int m_iSN;

    // total count
    static unsigned int m_iCount;

    // boolean for saving the collector is created from file or not
    bool m_bCreatingFromFile;

    // array for saving models such as svm
    std::vector<std::unique_ptr<mySVM>> m_vpoModel;

public:     // public method
    myModelCollector(void);
    myModelCollector(unsigned int iNumOfModel);
    virtual ~myModelCollector(void);

    void Resize(unsigned int iCount);
    void TrainModels(void);
    void SaveModels(const std::string& sRootPath) const;
    void LoadModels(std::string sFilePath);
    void AddSample(unsigned int iBinNumber, int iLabel,
                   const std::vector<float>& vfFeature);

protected:  // protected method

private:    // private method
    void Init(void);

};



#endif // !_MY_MODEL_COLLECTOR_H_

