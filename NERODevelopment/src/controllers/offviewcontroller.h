#ifndef OFFVIEWCONTROLLER_H
#define OFFVIEWCONTROLLER_H

#include "../models/model.h"
#include "../utils/attributestatus.h"
#include "../utils/descriptioninfo.h"
#include "buttoncontroller.h"
#include <QMap>
#include <QObject>
#include <QString>

class OffViewController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(QMap<QString, AttributeStatus> attributeStatus READ attributeStatus
                 NOTIFY attributeStatusChanged)
  Q_PROPERTY(float packTemp READ packTemp WRITE setPackTemp NOTIFY
                 packTempChanged FINAL)
  Q_PROPERTY(float motorTemp READ motorTemp WRITE setMotorTemp NOTIFY
                 motorTempChanged FINAL)
  Q_PROPERTY(float stateOfCharge READ stateOfCharge WRITE setStateOfCharge
                 NOTIFY stateOfChargeChanged FINAL)
  Q_PROPERTY(int selectedFlowIndex READ selectedFlowIndex WRITE
                 setSelectedFlowIndex NOTIFY selectedFlowIndexChanged FINAL)
  Q_PROPERTY(bool didSelect READ didSelect WRITE setDidSelect NOTIFY
                 didSelectChanged FINAL)
  Q_PROPERTY(QString selectedName READ selectedName WRITE setSelectedName NOTIFY
                 selectedNameChanged FINAL);
  Q_PROPERTY(
      QString selectedDescription READ selectedDescription WRITE
          setSelectedDescription NOTIFY selectedDescriptionChanged FINAL);
  Q_PROPERTY(QString selectedUrl READ selectedUrl WRITE setSelectedUrl NOTIFY
                 selectedUrlChanged FINAL);

public:
  explicit OffViewController(Model *model, QObject *parent = nullptr);

  QMap<QString, AttributeStatus> attributeStatus() const;
  float packTemp() const;
  float motorTemp() const;
  float stateOfCharge() const;
  int selectedFlowIndex() const;
  bool didSelect() const;
  QString selectedName() const;
  QString selectedDescription() const;
  QString selectedUrl() const;

signals:
  void
  attributeStatusChanged(const QMap<QString, AttributeStatus> &attributeStatus);
  void packTempChanged(float);
  void motorTempChanged(float);
  void stateOfChargeChanged(float);
  void didSelectChanged();
  void selectedFlowIndexChanged();
  void selectedNameChanged();
  void selectedDescriptionChanged();
  void selectedUrlChanged();

public slots:
  void setAttributeStatus(const QString &name, AttributeStatus status);
  void setPackTemp(float);
  void setMotorTemp(float);
  void setStateOfCharge(float);
  void setSelectedFlowIndex(int);
  void setDidSelect(bool);
  void setSelectedName(QString);
  void setSelectedDescription(QString);
  void setSelectedUrl(QString);

  void update();
  void enterButtonPressed() override;
  void downButtonPressed() override;
  void leftButtonPressed() override;
  void rightButtonPressed() override;
  void upButtonPressed() override;

private:
  AttributeStatus mapFloatToAttributeStatus(float);
  QMap<QString, AttributeStatus> m_attributeStatus;
  float m_packTemp;
  float m_motorTemp;
  float m_stateOfCharge;
  int m_selectedFlowIndex = 0;
  bool m_didSelect = 0;
  QString m_selectedName;
  QString m_selectedDescription;
  QString m_selectedUrl;

  QVector<DescriptionInfo> *m_descriptionInfo = new QVector<DescriptionInfo>{
      DescriptionInfo("Side BRBs", "", ""),
      DescriptionInfo("BMS", "", ""),
      DescriptionInfo("IMD", "", ""),
      DescriptionInfo("BSPD", "", ""),
      DescriptionInfo("BOTS", "", ""),
      DescriptionInfo("MPU", "", ""),
      DescriptionInfo("INERTIA", "", ""),
      DescriptionInfo("CRC BRB", "", ""),
      DescriptionInfo("TSMS", "", ""),
      DescriptionInfo("HVD INTERLOCK", "", ""),
      DescriptionInfo("HVD CONNECTOR", "", ""),

  };
};

#endif // OFFVIEWCONTROLLER_H
