#include "myModelCollector.h"

myModelCollector::myModelCollector(void) {
}

myModelCollector::myModelCollector(unsigned int iNumOfModel) {
    m_vpoModel.resize(iNumOfModel);
}

myModelCollector::myModelCollector(
    const cv::Mat & mImage, std::unique_ptr<myModelIndexerBase> poIndexer) {
    m_mImage = mImage;
    m_poIndexer = std::move(poIndexer);
    m_vpoModel.resize(m_poIndexer->GetNumOfBins());
}

myModelCollector::~myModelCollector(void) {}

void myModelCollector::TrainModels(void) {
    for (auto& model : m_vpoModel) {
        model->Train();
    }
}

void myModelCollector::SaveModels(void) {
}

void myModelCollector::LoadModels(std::string sFilePath) {
}

void myModelCollector::AddSample(cv::Point2i ptPosition, int iLabel) {
    auto BinNumber = m_poIndexer->GetBinNumber(ptPosition);
    std::vector<float> vfFeature;
    m_poExtractor->Describe(ptPosition, vfFeature);
    AddSample(BinNumber, iLabel, vfFeature);
}

void myModelCollector::AddSample(unsigned int iBinNumber,
                                 int iLabel,
                                 const std::vector<float>& vfFeature) {
    m_vpoModel.at(iBinNumber)->AddSample(iLabel, vfFeature);
}

void myModelCollector::Init(void) {
}
