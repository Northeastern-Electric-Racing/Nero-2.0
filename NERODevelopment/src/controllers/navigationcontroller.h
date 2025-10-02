#ifndef NAVIGATIONCONTROLLER_H
#define NAVIGATIONCONTROLLER_H

#include "../controllers/buttoncontroller.h"
#include "../models/model.h"
#include <QObject>

class NavigationController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(int selectedPageIndex READ selectedPageIndex WRITE
                 setSelectedPageIndex NOTIFY selectedPageIndexChanged FINAL)
  Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY
                 isSelectedChanged FINAL)
  Q_PROPERTY(bool isGamesOpen READ isGamesOpen WRITE setIsGamesOpen NOTIFY
                 isGamesOpenChanged FINAL)
  Q_PROPERTY(bool isThemeMenuOpen READ isThemeMenuOpen WRITE setIsThemeMenuOpen
                 NOTIFY isThemeMenuOpenChanged FINAL)
  Q_PROPERTY(bool isTsOn READ isTsOn WRITE setIsTsOn NOTIFY isTsOnChanged FINAL)

public:
  explicit NavigationController(Model *model, QObject *parent = nullptr);
  int selectedPageIndex() const;
  bool isSelected() const;
  bool isGamesOpen() const;
  bool isThemeMenuOpen() const;
  bool isTsOn() const;

signals:
  void selectedPageIndexChanged();
  void isSelectedChanged();
  void isGamesOpenChanged();
  void isThemeMenuOpenChanged();
  void themeChanged(QString theme);
  void isTsOnChanged(bool);

public slots:
  void setIsSelected(bool);
  void setIsGamesOpen(bool);
  void setIsThemeMenuOpen(bool);
  void setSelectedPageIndex(int);
  void setIsTsOn(bool);
  void currentDataDidChange();
  void enterButtonPressed() override;
  void downButtonPressed() override;
  void upButtonPressed() override;
  void homeButtonPressed() override;
  void buttonUpdate() override;
  void exitProgram();

private:
  bool m_isSelected = false;
  bool m_gamesSelected = false;
  bool m_themeMenuOpen = false;
  bool m_isTsOn;
  int m_selectedPageIndex = 0;
  int m_numPages = 8;
  int m_numGames = 2;
  int m_numThemes = 2;
};

#endif // NAVIGATIONCONTROLLER_H
