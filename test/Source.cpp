#include <opencv2/core.hpp>
#include "../myLibrary/myClassifier/mySVM/mySVM.h"

int main(void) {
    mySVM oSVM;
    oSVM.AddSample(1, std::vector<float>(10, 1.0f));
    oSVM.AddSample(-1, std::vector<float>(10, -1.0f));
    oSVM.Train();

    auto result = oSVM.Predict(std::vector<float>(10, -1.0f));
    oSVM.Save("test.xml");
    oSVM.Load("test.xml");
    result = oSVM.Predict(std::vector<float>(10, 1.0f));
	return 0;
}