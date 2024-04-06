#ifndef CONFIGURATIONCONTROLLER_H
#define CONFIGURATIONCONTROLLER_H

#include "buttoncontroller.h"

class ConfigurationController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(int selectedConfigurationIndex READ selectedConfigurationIndex
                 WRITE setSelectedConfigurationIndex NOTIFY
                     selectedConfigurationIndexChanged FINAL)
  Q_PROPERTY(bool isKeyboardSelected READ isKeyboardSelected WRITE
                 setIsKeyboardSelected NOTIFY isKeyboardSelectedChanged FINAL)

public:
  explicit ConfigurationController(Model *model, QObject *parent = nullptr);
  int selectedConfigurationIndex() const;
  bool isKeyboardSelected() const;

signals:
  void selectedConfigurationIndexChanged();
  void isKeyboardSelectedChanged();

public slots:
  void setSelectedConfigurationIndex(int);
  void setIsKeyboardSelected(bool);
  void setDriverName(QString);
  void setSystemName(QString);
  void setLocationName(QString);

  void downButtonPressed() override;
  void upButtonPressed() override;
  void enterButtonPressed() override;
  void homeButtonPressed() override;

private:
  int m_selectedConfigurationIndex = 0;
  bool m_isKeyboardSelected = false;
  QString m_driverName;
  QString m_systemName;
  QString m_locationName;
  int m_numConfigs = 4;
};

#endif // CONFIGURATIONCONTROLLER_H
