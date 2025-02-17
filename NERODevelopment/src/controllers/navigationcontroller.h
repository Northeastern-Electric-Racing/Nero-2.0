#ifndef NAVIGATIONCONTROLLER_H
#define NAVIGATIONCONTROLLER_H

#include "src/controllers/buttoncontroller.h"
#include "src/models/model.h"
#include <QObject>

class NavigationController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(int selectedPageIndex READ selectedPageIndex WRITE
                 setSelectedPageIndex NOTIFY selectedPageIndexChanged FINAL)
  Q_PROPERTY(bool isSelected READ isSelected WRITE setIsSelected NOTIFY
                 isSelectedChanged FINAL)
  Q_PROPERTY(bool isGamesOpen READ isGamesOpen WRITE setIsGamesOpen NOTIFY
                 isGamesOpenChanged FINAL)

public:
  explicit NavigationController(Model *model, QObject *parent = nullptr);
  int selectedPageIndex() const;
  bool isSelected() const;
  bool isGamesOpen() const;

signals:
  void selectedPageIndexChanged();
  void isSelectedChanged();
  void isGamesOpenChanged();

public slots:
  void setIsSelected(bool);
  void setIsGamesOpen(bool);
  void setSelectedPageIndex(int);
  void enterButtonPressed() override;
  void downButtonPressed() override;
  void upButtonPressed() override;
  void homeButtonPressed() override;
  void buttonUpdate() override;
  void exitProgram();

private:
  bool m_isSelected = false;
  bool m_gamesSelected = false;
  int m_selectedPageIndex = 0;
  int m_numPages = 7;
  int m_numGames = 2;
};

#endif // NAVIGATIONCONTROLLER_H
