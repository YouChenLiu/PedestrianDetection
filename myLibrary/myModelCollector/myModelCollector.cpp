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

void myModelCollector::SaveModels(const std::string& sRootPath) const {
    // buffer for folder string
    std::stringstream ssFolder;
    ssFolder << std::setfill('0') << std::setw(4) << m_iSN;

    // buffer for path string
    std::stringstream ssPath;
    ssPath << sRootPath << "/" << ssFolder.str();
    
    // buffer for command string
    std::stringstream ssCommand;
    ssCommand << "mkdir " << "\"" << ssPath.str() << "\"";
    std::string t = ssCommand.str();
    
    // create a sub folder at sRootPath
    std::system(ssCommand.str().c_str());

    // open file for write the xml file list
    std::ofstream FileList(sRootPath + "/" + "list.txt", std::ios_base::app);

    // add a line for xml file list
    FileList << ssFolder.str() << "/" << "list.txt" << std::endl;

    // open the file list in the sub folder, then write out the xml file path
    FileList.close();
    FileList.open(ssPath.str() + "/" + "list.txt");

    // write out the svm models
    for (std::size_t i = 0; i < m_vpoModel.size(); i++) {
        // buffer for file name
        std::stringstream ssFileName;
        ssFileName << std::setfill('0') << std::setw(3) << i << ".xml";

        // create the path for saving model
        std::stringstream ssXMLPath;
        ssXMLPath << ssPath.str() << "/" << ssFileName.str();

        // save the model and add a line in file list
        if (m_vpoModel.at(i)->Save(ssXMLPath.str())) {
            FileList << ssFileName.str() << std::endl;
        }
    }
}

void myModelCollector::LoadModels(std::string sFilePath) {
    

}

void myModelCollector::AddSample(unsigned int iBinNumber,
                                 int iLabel,
                                 const std::vector<float>& vfFeature) {
    m_vpoModel.at(iBinNumber)->AddSample(iLabel, vfFeature);
}

void myModelCollector::Init(void) {
    m_iSN = m_iCount++;
}
