#include <opencv2/core.hpp>
#include "../myLibrary/myFeatureExtractor/myLBP/myLBP.h"

int main(void) {
	myLBP LBP(cv::Mat(), myLBP::Feature::LBP_8_1_UNIFORM);
	return 0;
}