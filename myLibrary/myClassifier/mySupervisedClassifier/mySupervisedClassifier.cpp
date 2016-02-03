#include "mySupervisedClassifier.h"

mySupervisedClassifier::mySupervisedClassifier() {}

mySupervisedClassifier::~mySupervisedClassifier() {}

void mySupervisedClassifier::AddSample(int iLable,
                                       const std::vector<float>& vfFeature) {
    AddFeature(vfFeature);
    AddLabel(iLable);
}

void mySupervisedClassifier::SaveFeature(const std::string & sDstPath) const {
    // create file for writing
    std::ofstream File(sDstPath);

    // set flag let system print positive symbol
    File.setf(std::ios::showpos);

    // write out label and feature table together
    for (size_t i = 0; i < m_viLabel.size(); i++) {
        // write out label
        SaveLabel(File, m_viLabel.at(i));

        // string for represneting feature
        std::string s;

        // tranform feature to string
        WriteFeatureToString(m_vvfFeature.at(i), s);

        // write out feature string
        myClassifierBase::SaveFeature(File, s);
    }
}

void mySupervisedClassifier::LoadFeature(const std::string& sFeatureFile) {
    // open feature file
    std::ifstream File(sFeatureFile);

    // string for saving read text
    std::string s;
    while (std::getline(File, s)) {
        // tranform string to stream for convenience
        std::stringstream ss(s);

        // integer for saving label
        int iLabel = 0;

        // character for remove ':'
        char cSpilter;

        // get label
        ss >> iLabel;

        // remove ';'
        ss >> cSpilter;

        // residue string contains features
        ss >> s;

        // add a sample to table
        AddSample(iLabel, s);
    }

    File.close();
}

void mySupervisedClassifier::AddSample(int iLable,
                                       const std::string& sLoneOfString) {
    AddLabel(iLable);
    AddFeature(sLoneOfString);
}
