/**
 * @file mySVM.h
 * @brief Support Vector Machine class definition.
 */

#ifndef _MY_SVM_H_
#define _MY_SVM_H_

#include "../mySupervisedClassifier/mySupervisedClassifier.h"

using cv::ml::ParamGrid;
using cv::ml::SVM;

/**
 * @brief SVM classifer
 *
 * mySVM is a warped class.
 * It let's use opencv svm more convenience.
 */
class mySVM final : public mySupervisedClassifier {
public:     //public attribute
    /// encapsulate parameters for svm optimization problem
    class myOptimalParam {
    public: // public arrtibute
        /**
         * For SVM::C_SVC, SVM::EPS_SVR or SVM::NU_SVR.
         * The default value is 1.0.
         */
        double C;

        /**
         * For SVM::NU_SVC, SVM::ONE_CLASS or SVM::NU_SVR.
         * default value is 0.0.
         */
        double Nu;

        /**
         * For SVM::EPS_SVR.
         * The default value is 0.0.
         */
        double P;

    public: // public method
        /**
         * @brief Default contrunctor.
         *
         * Set parameters with default value.
         */
        myOptimalParam(void) {
            C = 1.0; Nu = P = 0.0;
        }

        /**
         * @brief Instance with specific value
         */
        myOptimalParam(double c, double nu, double p) {
            C = c; Nu = nu; P = p;
        }
    };
    
    /// encapsulate parameters for svm kernel function
    class myFunctionParam {
    public:
        /**
         * For SVM::POLY.
         * The default value is 0.0.
         */
        double Degree;

        /** For SVM::POLY, SVM::RBF, SVM::SIGMOID or SVM::CHI2.
         * default value is 1.0.
         */
        double Gamma;

        /**
         * For SVM::POLY or SVM::SIGMOID.
         * default value is 0.0
         */
        double Coef0;

    public:
        /**
        * @brief Default contrunctor
        *
        * Set parameters with default value.
        */
        myFunctionParam(void) {
            Degree = Coef0 = 0.0; Gamma = 1.0;
        }

        /**
        * @brief Instance with specific value
        */
        myFunctionParam(double degree, double gamma, double coef0) {
            Degree = degree; Gamma = gamma; Coef0 = coef0;
        }
    };

private:    // private attribute
    // ------------------------set SVM attributes---------------------------

    /**
     * SVM type will determine how to do classification.
     * the value can be C_SVC, NU_SVC,  etc...
     */
    static const int SVM_Type = cv::ml::SVM::C_SVC;

    /**
     * SVM kernel type will determine the classification method.
     * the value can be LINEAR, POLY, RBF, etc...
     */
    static const int SVM_KERNEL_Type = cv::ml::SVM::LINEAR;

    /**
     * This attribute determine what's situation can stop training.
     * COUNT : stop training when reach the max count.
     * MAX_ITER : stop training wehn reach max count or converge.
     * EPS : stop training when it's converge.
     */
    static const int CRITERIA_TYPE = cv::TermCriteria::MAX_ITER;

    /// the maximum training cycle count
    static const int ITERATION_COUNT = 10000;

    /** 
     * The system is converge when diffrence less than it.
     * initialize in cpp file
     */
    static const double EPSILON;

    // ------------------------end SVM attributes---------------------------

public:     //public method
    /// Create a svm with default parameter
    mySVM(void);

    /**
     * @brief Create a SVM with specify parameter encapsulated in two object.
     *
     * @param FParam The SVM attribute for kernel function.
     * @param OParam The SVM attribute for optimization problem.
     */
    mySVM(myFunctionParam FParam, myOptimalParam OParam);

    /**
     * @brief Create a SVM from xml file.
     *
     * @param sFilePath The destination xml file path.
     */
    mySVM(const std::string& sFilePath) {
        Load(sFilePath);
    }

    virtual ~mySVM(void);

    /**
     * @brief Load SVM saved in xml file.
     *
     * @param sFilePath The XML file path for reading.
     */
    void Load(const std::string& sFilePath) override {
        m_poClassifier = cv::ml::StatModel::load<cv::ml::SVM>(sFilePath);
    }
    
    /**
     * @brief Warp the OpenCV SVM function.
     *
     * This method warp the svm trainAuto method.
     * To see the opencv website for the detail.
     * <a href="http://docs.opencv.org/master/d1/d2d/
     * classcv_1_1ml_1_1SVM.html#ae612bc05c2e3e6b21fdff95279d32a73">Link</a>
     */
    bool TrainAuto(
        int kFold = 10,
        ParamGrid   Cgrid       = SVM::getDefaultGrid(SVM::C),
        ParamGrid   gammaGrid   = SVM::getDefaultGrid(SVM::GAMMA),
        ParamGrid   pGrid       = SVM::getDefaultGrid(SVM::P),
        ParamGrid   nuGrid      = SVM::getDefaultGrid(SVM::NU),
        ParamGrid   coeffGrid   = SVM::getDefaultGrid(SVM::COEF),
        ParamGrid   degreeGrid  = SVM::getDefaultGrid(SVM::DEGREE),
        bool        balanced    = false);

private:    // private method
    /// Initialize method
    void Init(void);

    /**
     * @brief Set SVM parameter for optimization problem.
     *
     * @param OParam The SVM attribute for optimization problem.
     */
    void SetOptimalParam(myOptimalParam Param);

    /**
     * Set SVM kernel function parameters.
     *
     * @param FParam The SVM attribute for kernel function.
     */
    void SetFunctionParam(myFunctionParam Param);

};

#endif // !_MY_SVM_H_