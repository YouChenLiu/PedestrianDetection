#include "mySVM.h"

namespace Classifier {

// the system is converge when diffrence less than it
const double mySVM::EPSILON = 1e-6;

mySVM::mySVM() {
  Init();
  SetOptimalParam(myOptimalParam());
  SetFunctionParam(myFunctionParam());
}

mySVM::mySVM(myFunctionParam FParam, myOptimalParam OParam) {
  Init();
  SetOptimalParam(OParam);
  SetFunctionParam(FParam);
}

mySVM::mySVM(myOptimalParam OParam) {
  Init();
  SetOptimalParam(OParam);
  SetFunctionParam(myFunctionParam());
}

mySVM::~mySVM() {}

bool mySVM::TrainAuto(int kFold, ParamGrid Cgrid, ParamGrid gammaGrid,
                      ParamGrid pGrid, ParamGrid nuGrid, ParamGrid coeffGrid,
                      ParamGrid degreeGrid, bool balanced) {
  
  auto pSVM = m_poClassifier.dynamicCast<cv::ml::SVM>();
  return pSVM->trainAuto(MakeTrainingData(), kFold, Cgrid, gammaGrid, pGrid,
                         nuGrid, coeffGrid, degreeGrid, balanced);
}

float mySVM::GetDistance(const cv::Mat& mSample) const {
  return m_poClassifier->predict(mSample, cv::noArray(), cv::ml::SVM::RAW_OUTPUT);
}

void mySVM::Init(void) {
  // create SVM
  auto pSVM = cv::ml::SVM::create();
  m_poClassifier = pSVM;

  // setting the SVM attribute
  pSVM->setType(mySVM::SVM_Type);
  pSVM->setKernel(mySVM::SVM_KERNEL_Type);
  pSVM->setTermCriteria(
    cv::TermCriteria(mySVM::CRITERIA_TYPE,
                     mySVM::ITERATION_COUNT,
                     mySVM::EPSILON)
  );
}

void mySVM::SetOptimalParam(myOptimalParam Param) {
  auto pSVM = m_poClassifier.dynamicCast<cv::ml::SVM>();
  pSVM->setC(Param.C);
  pSVM->setNu(Param.Nu);
  pSVM->setP(Param.P);
}

void mySVM::SetFunctionParam(myFunctionParam Param) {
  auto pSVM = m_poClassifier.dynamicCast<cv::ml::SVM>();
  pSVM->setDegree(Param.Degree);
  pSVM->setGamma(Param.Gamma);
  pSVM->setCoef0(Param.Coef0);
}

} // namespace