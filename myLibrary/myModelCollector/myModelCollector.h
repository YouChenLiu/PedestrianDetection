#ifndef _MY_MODEL_COLLECTOR_H_
#define _MY_MODEL_COLLECTOR_H_

#include "myModelCollectorBase.h"
#include "../myClassifier/mySVM/mySVM.h"
#include <memory>

/**
 * @brief Class processes classifiers behavior.
 */
class myModelCollector final : public myModelCollectorBase {
public:     // public attribute

protected:  // protected attribute

private:    // private attribute
    /// serial number
    int m_iSN;

    /// total count
    static unsigned int m_iCount;

    /// boolean for saving the collector is created from file or not
    bool m_bCreatingFromFile;

    /// array for saving models such as svm
    std::vector<std::unique_ptr<mySVM>> m_vpoModel;

public:     // public method
    /**
     * @brief Deafult Constructor.
     */
    myModelCollector(void);

    /**
     * @brief Constructor.
     *
     * @param iNumOfModel The models count.
     */
    myModelCollector(unsigned int iNumOfModel);
    virtual ~myModelCollector(void);

    /**
     * @brief Resize the vector to specific number and renew models.
     *
     * @param iCount The new models count want to set.
     */
    void Resize(unsigned int iCount);

    /**
     * @brief Train the all models in collector.
     */
    void TrainModels(void);

    /**
     * @brief Save all models in collector to xml files.
     *
     * @return The list file path for reconstruct collector.
     */
    std::string SaveModels(const std::string& sRootPath) const;

    /**
     * @brief Save all features at each models in collector to text files.
     *
     * @return List file path for reconstruct.
     */ 
    std::string SaveFeatures(const std::string& sRootPath) const;
    
    /**
     * @brief Reconstruct the model by xml file.
     *
     * @param sFilePath The list file path for reading.
     */
    void LoadModels(const std::string& sFilePath);

    /**
     * @brief Reconstruct the model by features saved in text file.
     *
     * @param sFilePath The list file path for reading.
     */ 
    void LoadFeatures(const std::string& sFilePath);

    /**
     * @brief Add a sample with label and feature vector to specific model.
     * 
     * @param iBinNumber The model number.
     * @param iLabel The label describes feature belongs to which class.
     * @param vfFeature The feature wanted to add to table.
     */
    void AddSample(unsigned int iBinNumber, int iLabel,
                   const std::vector<float>& vfFeature);

    /**
     * @brief Predict by a Model
     * 
     * @param iBinNumber Specified model number.
     * @param mSample A Sample in Mat.
     */
    float Predict(unsigned int iBinNumber, cv::Mat& mSample) const {
        return m_vpoModel.at(iBinNumber)->Predict(mSample);
    }

    /**
     * @brief Predict by a Model
     *
     * @param iBinNumber Specified model number.
     * @param vfSample A sample in std::vector.
     */
    float Predict(unsigned int iBinNumber, std::vector<float>& vfSample) const {
        return m_vpoModel.at(iBinNumber)->Predict(vfSample);
    }

protected:  // protected method

private:    // private method
    void Init(void);

    // create a folder for saving files
    std::string CreateFolder(const std::string& sRootPath) const;
};

#endif // !_MY_MODEL_COLLECTOR_H_

