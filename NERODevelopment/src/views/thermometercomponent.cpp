#include "thermometercomponent.h"

ThermometerComponent::ThermometerComponent(QObject *parent)
    : QObject{parent}, m_maxValue(0.0), m_minValue(0.0) {}

float ThermometerComponent::maxValue() const { return m_maxValue; }

void ThermometerComponent::setMaxValue(float maxValue) {
  if (maxValue != m_maxValue) {
    m_maxValue = maxValue;
    emit maxValueChanged(maxValue);
  }
}

float ThermometerComponent::minValue() const { return m_minValue; }

void ThermometerComponent::setMinValue(float minValue) {
  if (minValue != m_minValue) {
    m_minValue = minValue;
    emit minValueChanged(minValue);
  }
}
