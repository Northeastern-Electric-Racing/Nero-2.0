#ifndef KEYBOARDCONTROLLER_H
#define KEYBOARDCONTROLLER_H

#include "buttoncontroller.h"

class KeyboardController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY
                 selectedIndexChanged FINAL)
  Q_PROPERTY(bool selectedKey READ selectedKey WRITE setSelectedKey NOTIFY
                 selectedKeyChanged FINAL)
public:
  explicit KeyboardController(Model *model, QObject *parent = nullptr);

  int selectedIndex() const;
  bool selectedKey() const;

signals:
  void selectedIndexChanged();
  void selectedKeyChanged();

public slots:
  void setSelectedIndex(int);
  void setSelectedKey(bool);

  void downButtonPressed() override;
  void upButtonPressed() override;
  void leftButtonPressed() override;
  void rightButtonPressed() override;
  void enterButtonPressed() override;

private:
  int m_selectedIndex = 0;
  bool m_selectedKey = false;
  int m_numKeys = 45;
  int m_columns = 10;
};

#endif // KEYBOARDCONTROLLER_H
