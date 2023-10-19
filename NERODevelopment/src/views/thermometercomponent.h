#ifndef THERMOMETERCOMPONENT_H
#define THERMOMETERCOMPONENT_H

#include <QObject>

class ThermometerComponent : public QObject {
  Q_OBJECT
  Q_PROPERTY(float maxValue READ maxValue WRITE setMaxValue NOTIFY
                 maxValueChanged FINAL)
  Q_PROPERTY(float minValue READ minValue WRITE setMinValue NOTIFY
                 minValueChanged FINAL)

public:
  explicit ThermometerComponent(QObject *parent = nullptr);
  float maxValue() const;
  float minValue() const;

signals:
  void maxValueChanged(float);
  void minValueChanged(float);

public slots:
  void setMaxValue(float);
  void setMinValue(float);

private:
  float m_maxValue;
  float m_minValue;
};

#endif // THERMOMETERCOMPONENT_H
