#include "../myLibrary/myClassifier/myAdaBoost/myAdaBoost.h"

int main(void) {
    //mySVM oClassifier();
    myAdaBoost oClassifier(5);
    for (std::size_t i = 0; i < 10; ++i) {
        oClassifier.AddSample(+1, std::vector<float>(10, static_cast<float>(i)));
        oClassifier.AddSample(-1, std::vector<float>(10, -static_cast<float>(i)));
    }
    
    oClassifier.SaveFeature("test.txt");
    oClassifier.Train();
    oClassifier.LoadFeature("test.txt");
    auto result = oClassifier.Predict(std::vector<float>(10, -1.1f));
    oClassifier.Save("test.xml");
    oClassifier.Load("test.xml");
    result = oClassifier.Predict(std::vector<float>(10, 1.1f));
	return 0;
}