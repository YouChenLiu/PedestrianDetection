#include "myLBP.h"

std::array<std::vector<bool>, myLBP::MAX_BIT_LENGTH / 8> myLBP::m_avbUniformMap = {};
std::array<std::vector<cv::Point2i>, myLBP::NUMBER_OF_PATTERNS> myLBP::m_SamplingPoints = {};

myLBP::myLBP(const cv::Mat& mImage, int Pattern, cv::Size2i blockSize) {
    Init();
    m_mImage = mImage.clone();
    m_iPattern = Pattern;
    m_BlockSize = blockSize;
    SetAttributes(Pattern);
}

myLBP::~myLBP(void) {}

void myLBP::Init(void) {
    m_mImage = cv::Mat();
    m_bIsUniform = false;
    m_iRadius = 0;
    m_iLength = 0;

    if (m_SamplingPoints.at(0).empty() == true) {
        SetSamplingPoints();
        for (int iLength = 8; iLength <= myLBP::MAX_BIT_LENGTH; iLength += 8) {
            int iUniformMapIndex = iLength / 8 - 1;
            myLBP::m_avbUniformMap.at(iUniformMapIndex).resize(static_cast<int>(pow(2, iLength)));
            for (int j = 0; j < static_cast<int>(m_avbUniformMap.at(iUniformMapIndex).size()); ++j) {
                myLBP::m_avbUniformMap.at(iUniformMapIndex).at(j) = ((IsUniform(j, iLength) == true) ? true : false);
            }
        }
    }
}

void myLBP::SetSamplingPoints(void) {
    using namespace cv;
    std::array<Point2i, 8> Location8_1 = {
        Point2i(-1, -1), Point2i(0, -1), Point2i(+1, -1), Point2i(+1, +0),
        Point2i(+1, +1), Point2i(0, +1), Point2i(-1, +1), Point2i(-1, +0),
    };
    for (auto pt : Location8_1) {
        m_SamplingPoints.at(Feature::LBP_8_1).push_back(pt);
    }

    std::array<Point2i, 8> Location8_2 = {
        Point2i(-2, -2), Point2i(+0, -2), Point2i(+2, -2), Point2i(+2, +0), 
        Point2i(+2, +2), Point2i(+0, +2), Point2i(-2, +2), Point2i(-2, +0)
    };
    for (auto pt : Location8_2) {
        m_SamplingPoints.at(Feature::LBP_8_2).push_back(pt);
    }

    std::array<Point2i, 16> Location16_2 = {
        Point2i(-2, -2), Point2i(-1, -2), Point2i(+0, -2), Point2i(+1, -2), Point2i(+2, -2), Point2i(+2, -1), Point2i(+2, +0), Point2i(+2, +1),
        Point2i(+2, +2), Point2i(+1, +2), Point2i(+0, +2), Point2i(-1, +2), Point2i(-2, +2), Point2i(-2, +1), Point2i(-2, +0), Point2i(-2, -1)
    };
    for (auto pt : Location16_2) {
        m_SamplingPoints.at(Feature::LBP_16_2).push_back(pt);
    }
}

void myLBP::SetAttributes(int iPattern) {
    m_bIsUniform = ((iPattern >= myLBP::NUMBER_OF_PATTERNS) ? true : false);
    m_iRadius = ((iPattern % myLBP::NUMBER_OF_PATTERNS == 0) ? 1 : 2);
    m_iLength = ((iPattern % myLBP::NUMBER_OF_PATTERNS == 2) ? 16 : 8);
}

void myLBP::Describe(cv::Point2i Position, std::vector<float>& viFeature) const {
    int iUniformMapIndex = m_iLength / 8 - 1;
    std::vector<float> viTempBins(static_cast<int>(m_avbUniformMap.at(iUniformMapIndex).size()), 0.0f);

    for (int y = Position.y; y < Position.y + m_BlockSize.height; ++y) {
        for (int x = Position.x; x < Position.x + m_BlockSize.width; ++x) {
            ++viTempBins.at(GetBinNumber(cv::Point2i(x, y)));
        }
    }

    viFeature.clear();
    float iNonuniformBin = 0;
    for (int i = 0; i < static_cast<int>(myLBP::m_avbUniformMap.at(iUniformMapIndex).size()); ++i) {
        if (m_bIsUniform == true) {
            if (myLBP::m_avbUniformMap.at(iUniformMapIndex).at(i) == true) {
                viFeature.push_back(viTempBins.at(i));
            } else {
                iNonuniformBin += viTempBins.at(i);
            }
        } else {
            viFeature.push_back(viTempBins.at(i));
        }
    }

    if (m_bIsUniform == true) {
        viFeature.push_back(iNonuniformBin);
    }
}

unsigned int myLBP::GetBinNumber(cv::Point2i Position) const {
    unsigned int iBinNumber = 0;

    const auto cCenterIntensity = m_mImage.at<unsigned char>(Position);
    const auto& SampleingPoints = m_SamplingPoints.at(static_cast<int>(m_iPattern) % myLBP::NUMBER_OF_PATTERNS);
    for (auto pt : SampleingPoints) {
        auto CurrentPosition = Position + pt;
        if (CurrentPosition.x < 0 || CurrentPosition.y < 0 || CurrentPosition.x >= (m_mImage.cols - 1) || CurrentPosition.y >= (m_mImage.rows)) {
            continue;
        }
        auto cCurrentIntensity = m_mImage.at<unsigned char>(CurrentPosition);
        iBinNumber = (iBinNumber << 1) | ((cCurrentIntensity <= cCenterIntensity) ? 0x00000000 : 0x00000001);
    }

    return iBinNumber;
}

bool myLBP::IsUniform(unsigned int iBinNumber, int iLength) {
    bool bResult = true;

    int iChangeTime = 0;
    unsigned int iMaskBit = 0x00000001;
    unsigned int iCheckBit = iBinNumber & iMaskBit;

    for (int i = 0; i < iLength - 1; ++i) {
        iMaskBit <<= 1;
        iCheckBit <<= 1;
        unsigned int iComparedValue = (iMaskBit & iBinNumber);
        if (iComparedValue != iCheckBit) {
            iCheckBit = iComparedValue;
            ++iChangeTime;
            if (iChangeTime > myLBP::MAX_TRANSITION_TIME) {
                bResult = false;
                break;
            }
        }
    }

    return bResult;
}

#ifdef _DEBUG
void myLBP::PrintUniformMap(int iLength) const {
    int i = 0;
    int sum = 0;
    for (auto bIsUnoform : m_avbUniformMap.at(iLength / 8 - 1)) {
        std::cout << std::setw(3) << i++ << ":" << (bIsUnoform ? "T" : "F") << std::endl;
        if (bIsUnoform == true) {
            ++sum;
        }
    }
    std::cout << sum << std::endl;
}
#endif