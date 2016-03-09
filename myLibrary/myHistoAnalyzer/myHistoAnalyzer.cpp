#include "myHistoAnalyzer.h"

myHistoAnalyer::myHistoAnalyer(const std::vector<float>& vfHistogram,
                               bool bNormalized) {
    m_vfHistogram = vfHistogram;

    if (!bNormalized) {
        myHistoAnalyer::Normalize(m_vfHistogram);
    }
}

myHistoAnalyer::~myHistoAnalyer(void) {}

float myHistoAnalyer::CalculateDistance(const myHistoAnalyer & obj,
                                        Distance Method) const {
    float fDistance = 0.0f;

    // check the length are same
    if (m_vfHistogram.size() != obj.GetLength()) {
        return NAN;
    }

    // compute distance
    for (size_t i = 0; i < m_vfHistogram.size(); ++i) {
        // ith self element
        auto f1 = m_vfHistogram.at(i);

        // ith obj element
        auto f2 = obj.GetElement(i);

        switch (Method) {
        case myHistoAnalyer::Distance::BHATTACHARYYA:
            fDistance += sqrt(f1 * f2);
            break;
        case myHistoAnalyer::Distance::OVERLAP:
            fDistance += (f1 > f2) ? f2 : f1;
            break;
        default:
            std::cout << "Invalid Method" << std::endl;
            break;
        }
        
    }

    return fDistance;
}

void myHistoAnalyer::Normalize(std::vector<float>& vfHistogram) {
    float fSum = 0.0f;

    for (auto f : vfHistogram) {
        fSum += f;
    }

    if (fSum == 0.0f) {
        std::cout << "Can't Normalize." << std::endl;
        return;
    }

    for (auto& f : vfHistogram) {
        f /= fSum;
    }
}