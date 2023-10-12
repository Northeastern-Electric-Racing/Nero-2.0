#include "thermometerview.h"

thermometerview::thermometerview(QObject *parent)
    : QObject{parent}
    , m_maxValue(0.0)
    , m_minValue(0.0)
{
}

float thermometerview::maxValue() const
{
    return m_maxValue;
}

void thermometerview::setMaxValue(float maxValue)
{
    if (maxValue != m_maxValue) {
        m_maxValue = maxValue;
        emit maxValueChanged(maxValue);
    }
}

float thermometerview::minValue() const
{
    return m_minValue;
}

void thermometerview::setMinValue(float minValue)
{
    if (minValue != m_minValue) {
        m_minValue = minValue;
        emit minValueChanged(minValue);
    }
}
