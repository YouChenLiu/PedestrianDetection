#ifndef _MY_SVM_H_
#define _MY_SVM_H_

#include "../mySupervisedClassifier/mySupervisedClassifier.h"

using cv::ml::ParamGrid;
using cv::ml::SVM;

// mySVM is a warped class
// let's use opencv svm more convenience
class mySVM final : public mySupervisedClassifier {
public:
    // encapsulate parameters for svm optimization problem
    class myOptimalParam {
    public:
        // For SVM::C_SVC, SVM::EPS_SVR or SVM::NU_SVR
        // default value is 0.0
        double C;

        // For SVM::NU_SVC, SVM::ONE_CLASS or SVM::NU_SVR
        // default value is 0.0
        double Nu;

        // For SVM::EPS_SVR
        // default value is 0.0
        double P;

    public:
        myOptimalParam(void) {
            C = Nu = P = 0.0;
        }
        myOptimalParam(double c, double nu, double p) {
            C = c; Nu = nu; P = p;
        }
    };
    
    // encapsulate parameters for svm kernel function
    class myFunctionParam {
    public:
        // For SVM::POLY
        // default value is 0.0
        double Degree = 0.0;

        // For SVM::POLY, SVM::RBF, SVM::SIGMOID or SVM::CHI2
        // default value is 1.0
        double Gamma = 1.0;

        // For SVM::POLY or SVM::SIGMOID
        // default value is 0.0
        double Coef0 = 0.0;

    public:
        myFunctionParam(void) {
            Degree = Coef0 = 0.0; Gamma = 1.0;
        }

        myFunctionParam(double degree, double gamma, double coef0) {
            Degree = degree; Gamma = gamma; Coef0 = coef0;
        }
    };

public:
    // create a svm with default parameter
    mySVM(void);

    // create a svm with specify parameter encapsulated in two object
    mySVM(myFunctionParam FParam, myOptimalParam OParam);

    // create a svm from xml file
    mySVM(const std::string& sFilePath) {
        Load(sFilePath);
    }

    virtual ~mySVM(void);

    // load SVM saved in xml file
    void Load(const std::string& sFilePath) override {
        m_poClassifier = cv::ml::StatModel::load<cv::ml::SVM>(sFilePath);
    }
    
    // this warp the svm trainAuto method
    // see the opencv website for the detail
    bool TrainAuto(
        int kFold = 10,
        ParamGrid   Cgrid       = SVM::getDefaultGrid(SVM::C),
        ParamGrid   gammaGrid   = SVM::getDefaultGrid(SVM::GAMMA),
        ParamGrid   pGrid       = SVM::getDefaultGrid(SVM::P),
        ParamGrid   nuGrid      = SVM::getDefaultGrid(SVM::NU),
        ParamGrid   coeffGrid   = SVM::getDefaultGrid(SVM::COEF),
        ParamGrid   degreeGrid  = SVM::getDefaultGrid(SVM::DEGREE),
        bool        balanced    = false);

private:
    // ------------------------set SVM attributes---------------------------

    // SVM type will determine how to do classification
    // the value can be C_SVC, NU_SVC,  etc..
    static const int SVM_Type = cv::ml::SVM::C_SVC;

    // SVM kernel type will determine the classification method
    // the value can be LINEAR, POLY, RBF, etc..
    static const int SVM_KERNEL_Type = cv::ml::SVM::LINEAR;

    // this attribute determine what's situation can stop training
    // COUNT : stop training when reach the max count
    // MAX_ITER : stop training wehn reach max count or converge
    // EPS : stop training when it's converge
    static const int CRITERIA_TYPE = cv::TermCriteria::MAX_ITER;

    // the maximum training cycle count
    static const int ITERATION_COUNT = 10000;

    // the system is converge when diffrence less than it
    // initialize in cpp file
    static const double EPSILON;

    // opencv criteria object
    // initialize in cpp file
    static const cv::TermCriteria CRITERIA;

    // ------------------------end SVM attributes---------------------------

private:
    // initialization method
    void Init(void);

    // set svm parameter of optimization problem
    void SetOptimalParam(myOptimalParam Param);

    // set svm parameter of kernel function
    void SetFunctionParam(myFunctionParam Param);

};

#endif // !_MY_SVM_H_