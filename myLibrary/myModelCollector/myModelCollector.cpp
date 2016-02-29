#include "myModelCollector.h"
#include <iomanip>
#include <fstream>

unsigned int myModelCollector::m_iCount = 0;

myModelCollector::myModelCollector(void) {
    Init();
}

myModelCollector::myModelCollector(unsigned int iNumOfModel) {
    Init();
    Resize(iNumOfModel);
}

myModelCollector::~myModelCollector(void) {
    --m_iCount;
}

void myModelCollector::Resize(unsigned int iCount) {
    m_vpoModel.clear();
    m_vpoModel.reserve(iCount);
    for (size_t i = 0; i < iCount; ++i) {
        m_vpoModel.push_back(std::make_unique<mySVM>());
    }
}

void myModelCollector::TrainModels(void) {
    for (const auto& model : m_vpoModel) {
        model->Train();
    }
}

std::string myModelCollector::SaveModels(const std::string& sRootPath) const {
    std::string sSubFolder = CreateFolder(sRootPath);
    
    // string for list file path
    std::string sListFilePath = sRootPath + "/" + sSubFolder + "/models.txt";

    // open the file list in the sub folder, then write out the xml file path
    std::ofstream ListFile(sListFilePath);

    // write out the count of models
    ListFile << m_vpoModel.size() << std::endl;

    // write out the svm models
    for (std::size_t i = 0; i < m_vpoModel.size(); i++) {
        // buffer for xml file name
        std::stringstream ssXMLFileName;
        ssXMLFileName << std::setfill('0') << std::setw(3) << i << ".xml";

        // create the path for saving model
        std::stringstream ssXMLPath;
        ssXMLPath << sRootPath << "/"
            << sSubFolder << "/"
            << ssXMLFileName.str();

        // save the model and add a line in file list
        if (m_vpoModel.at(i)->Save(ssXMLPath.str())) {
            ListFile << ssXMLFileName.str() << std::endl;
        }
    }

    return sListFilePath;
}

std::string myModelCollector::SaveFeatures(const std::string& sRootPath) const {
    std::string sSubFolder = CreateFolder(sRootPath);
    
    // string for list file path
    std::string sListFilePath = sRootPath + "/" + sSubFolder + "/features.txt";

    // open the file list in the sub folder, then write out the xml file path
    std::ofstream ListFile(sListFilePath);

    // write out the count of models
    ListFile << m_vpoModel.size() << std::endl;

    // write out the svm models
    for (std::size_t i = 0; i < m_vpoModel.size(); i++) {
        // buffer for feature file name
        std::stringstream ssFileName;
        ssFileName << std::setfill('0') << std::setw(3) << i << ".txt";

        // buffer for feature file path
        std::stringstream ssFeaturePath;
        ssFeaturePath << sRootPath << "/"
            << sSubFolder << "/"
            << ssFileName.str();

        // write out the features
        m_vpoModel.at(i)->SaveFeatures(ssFeaturePath.str());

        // add path to list file
        ListFile << ssFileName.str() << std::endl;
    }

    return sListFilePath;
}

void myModelCollector::LoadModels(const std::string& sFilePath) {
    // open the model list file
    std::ifstream FileList(sFilePath);

    // read the models count
    std::string sModelCount;
    FileList >> sModelCount;

    // get total models count
    std::stringstream ssCount(sModelCount);
    unsigned int iCount = 0;
    ssCount >> iCount;
    Resize(iCount);

    // string for models file root path
    std::string sModelsRootPath = sFilePath;
    sModelsRootPath.resize(sFilePath.rfind('/') + 1);

    // read lines of text file
    for (std::size_t i = 0; i < iCount; ++i) {
        std::string sLine;
        FileList >> sLine;
        if (sLine.size() == 0) {
            break;
        }
        std::stringstream ss(sLine);
        int iModelNumber = 0;
        ss >> iModelNumber;
        m_vpoModel.at(iModelNumber)->Load(sModelsRootPath + sLine);
    }
}

void myModelCollector::LoadFeatures(const std::string & sFilePath) {
    // open the model list file
    std::ifstream FileList(sFilePath);

    // read the models count
    std::string sModelCount;
    FileList >> sModelCount;

    // get total models count
    std::stringstream ssCount(sModelCount);
    unsigned int iCount = 0;
    ssCount >> iCount;
    Resize(iCount);

    // string for models file root path
    std::string sModelsRootPath = sFilePath;
    sModelsRootPath.resize(sFilePath.rfind('/') + 1);

    // read lines of text file
    for (std::size_t i = 0; i < iCount; ++i) {
        std::string sLine;
        FileList >> sLine;

        std::stringstream ss(sLine);
        int iModelNumber = 0;
        ss >> iModelNumber;
        m_vpoModel.at(iModelNumber)->LoadFeatures(sModelsRootPath + sLine);
    }
}

void myModelCollector::AddSample(unsigned int iBinNumber,
                                 int iLabel,
                                 const std::vector<float>& vfFeature) {
    m_vpoModel.at(iBinNumber)->AddSample(iLabel, vfFeature);
}

void myModelCollector::Init(void) {
    m_iSN = m_iCount++;
}

std::string myModelCollector::CreateFolder(const std::string& sRootPath) const {
    // buffer for folder string
    std::stringstream ssFolder;
    ssFolder << std::setfill('0') << std::setw(4) << m_iSN;

    // buffer for path string
    std::stringstream ssPath;
    ssPath << sRootPath << "/" << ssFolder.str();

    // buffer for command string
    std::stringstream ssCommand;
    ssCommand << "mkdir " << "\"" << ssPath.str() << "\"";

    // create a sub folder at sRootPath
    std::system(ssCommand.str().c_str());

    return ssFolder.str();
}