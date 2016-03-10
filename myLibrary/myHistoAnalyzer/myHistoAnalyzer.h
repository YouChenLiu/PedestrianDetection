/**
 * @file myHistoAnalyzer.h
 * @brief myHistoAnalyer definition.
 */

#ifndef _MY_HISTO_ANALYER_H_
#define _MY_HISTO_ANALYER_H_

#include "../common.h"

/**
 * @brief Histogram Analyer class
 *
 * myHistoAnalyer is a class for computing distance between two histogram.
 */
class myHistoAnalyer {
public:     // public attribute
    /**
     * @brief Enumeration class define distance computation method.
     */
    enum class Distance {
        BHATTACHARYYA,      //!< Bhattacharyya coefficient
        OVERLAP             //!< Overlapping
    };

protected:  // protected attribute

private:    // private attribute
    std::vector<float> m_vfHistogram;

public:     // public method
    /**
     * @brief Contructor
     *
     * @param vfHistogram A histogram.
     * @param bNormalized The histogram is normalized or not.
     */
    myHistoAnalyer(const std::vector<float>& vfHistogram, bool bNormalized);
    ~myHistoAnalyer(void);

    /**
     * @brief Calculate the distance between obj with specify method.
     *
     * @param obj Another HistoAnalyer object.
     * @param Method The method for distance calculation.
     * @retval NAN Can not calculate distance with obj.
     */
    float CalculateDistance(const myHistoAnalyer& obj, Distance Method) const;

    /**
     * @brief Get histogram length.
     *
     * @return The length of Histogram mantained by self.
     */
    std::size_t GetLength(void) const { return m_vfHistogram.size(); }

    /**
     * @brief Get histogram element by index i.
     *
     * @param i Index.
     * @return The ith element.
     */
    float GetElement(size_t i) const { return m_vfHistogram.at(i); }

protected:  // protected method

private:    // private method
    /**
     * @brief Normalize a histogram.
     *
     * @param vfHistogram A histogram wants to normalize.
     */
    static void Normalize(std::vector<float>& vfHistogram);
};

#endif // !_MY_HISTO_ANALYER_H_