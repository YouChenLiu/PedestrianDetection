#include "myClassifierBase.h"
#include <sstream>
#include <fstream>

void myClassifierBase::SaveFeature(std::string & sDstPath) {
    std::ofstream File(sDstPath);
    for (auto vf : m_vvfFeature) {
        std::stringstream ss;
        for (auto f : vf) {
            ss << f << ',';
        }
        std::string s = ss.str();
        s.substr(0, s.length() - 1);
        File << ss.str();
        File << std::endl;
    }
}

void myClassifierBase::LoadFeature(const std::string & sFeatureFile) {
    std::ifstream File(sFeatureFile);
    while (File.eof()) {
        std::string sLine;
        File >> sLine;
        AddFeature(sLine);
    }
}

void TranformFeature(const std::string& s, char delim, std::vector<float>& vfFeature) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        vfFeature.push_back(stof(item));
    }
}

void myClassifierBase::AddFeature(const std::string & sLineOfFeature) {
    std::vector<float> vfFeature;
    TranformFeature(sLineOfFeature, ',', vfFeature);
    AddFeature(vfFeature);
}
