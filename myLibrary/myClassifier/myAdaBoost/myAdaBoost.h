#ifndef _MY_ADABOOST_H_
#define _MY_ADABOOST_H_

#include "../mySupervisedClassifier/mySupervisedClassifier.h"

class myAdaBoost final : public mySupervisedClassifier {
public:
    myAdaBoost(unsigned int iWeakCount = 100);

    virtual ~myAdaBoost(void);

    void Load(const std::string& sFilePath) override {
        m_poClassifier = cv::ml::StatModel::load<cv::ml::Boost>(sFilePath);
    }

private:
    // AdaBoost Types
    // it can be the following values
    // DISCRETE, REAL(default), LOGIT, GENTLE
    static const int BOOST_TYPE = cv::ml::Boost::Types::REAL;

    // A threshold between 0 and 1 used to save computational time
    // training will stop when samples with summary weight <= 1 - TRIM_RATE
    // default value is 0.95, 0 for turning off
    static const double TRIM_RATE;

private:

};

#endif // !_MY_ADABOOST_H_
