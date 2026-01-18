#ifndef NAVIGATIONCONTROLLER_H
#define NAVIGATIONCONTROLLER_H

#include "../controllers/buttoncontroller.h"
#include "../models/model.h"

namespace Menu {
Q_NAMESPACE

enum class Type { Page, Category, Action, SubPage, SubAction };
Q_ENUM_NS(Type)

struct Item {
  const char *label;
  Type type;
  int row;
  const char *icon;
  const char *qml;
};

constexpr Item PAGES[] = {
    {"OFF", Type::Page, 1, "zzz.png", "OffScreen2.qml"},
    {"PIT - DRIVE", Type::Page, 1, "flag.png", "Pit.qml"},
    {"PIT - REVERSE", Type::Page, 1, "reverse.png", "Pit.qml"},
    {"PERFORMANCE", Type::Page, 1, "hare.png", "SpeedMode.qml"},
    {"EFFICIENCY", Type::Page, 1, "turtle.png", "EfficiencyScreen.qml"},
    {"GAMES", Type::Category, 2, "game.png", nullptr},
    {"FLAPPY BIRD", Type::SubPage, 2, nullptr, "FlappyBird.qml"},
    {"SNAKE", Type::SubPage, 2, nullptr, "Snake.qml"},
    {"THEMES", Type::Category, 2, "themes.png", nullptr},
    {"LIGHT", Type::SubAction, 2, nullptr, nullptr},
    {"DARK", Type::SubAction, 2, nullptr, nullptr},
    {"EXIT", Type::Action, 2, "exit.png", nullptr},
};

constexpr int COUNT = sizeof(PAGES) / sizeof(PAGES[0]);

constexpr bool valid(int i) { return i >= 0 && i < COUNT; }

constexpr bool isSubItem(int i) {
  return valid(i) &&
         (PAGES[i].type == Type::SubPage || PAGES[i].type == Type::SubAction);
}

constexpr bool isTopLevel(int i) { return valid(i) && !isSubItem(i); }

constexpr bool isCategory(int i) {
  return valid(i) && PAGES[i].type == Type::Category;
}

constexpr int firstChildOf(int parent) {
  if (!valid(parent) || !isCategory(parent))
    return -1;
  int next = parent + 1;
  return (valid(next) && isSubItem(next)) ? next : -1;
}

constexpr bool hasChildren(int i) { return firstChildOf(i) >= 0; }

inline QString label(int i) {
  return valid(i) && PAGES[i].label ? QString::fromUtf8(PAGES[i].label)
                                    : QString();
}

inline QString icon(int i) {
  if (!valid(i) || !PAGES[i].icon)
    return QString();
  return QStringLiteral("/qt/qml/content/images/%1")
      .arg(QString::fromUtf8(PAGES[i].icon));
}

inline QString qml(int i) {
  return valid(i) && PAGES[i].qml ? QString::fromUtf8(PAGES[i].qml) : QString();
}

inline QVector<int> topLevelInRow(int row) {
  QVector<int> r;
  for (int i = 0; i < COUNT; ++i)
    if (isTopLevel(i) && PAGES[i].row == row)
      r.append(i);
  return r;
}

inline QVector<int> childrenOf(int parent) {
  QVector<int> r;
  if (!isCategory(parent))
    return r;
  for (int i = parent + 1; i < COUNT && isSubItem(i); ++i)
    r.append(i);
  return r;
}
} // namespace Menu

class NavigationController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY
                 selectionChanged)
  Q_PROPERTY(int activePageIndex READ activePageIndex NOTIFY activePageChanged)
  Q_PROPERTY(int expandedIndex READ expandedIndex NOTIFY expandedChanged)
  Q_PROPERTY(bool isPageActive READ isPageActive NOTIFY activePageChanged)
  Q_PROPERTY(bool isTsOn READ isTsOn NOTIFY isTsOnChanged)

public:
  explicit NavigationController(Model *model, QObject *parent = nullptr);

  int selectedIndex() const { return m_selected; }
  int activePageIndex() const { return m_activePage; }
  int expandedIndex() const { return m_expanded; }
  bool isPageActive() const { return m_activePage >= 0; }
  bool isTsOn() const { return m_isTsOn; }

  Q_INVOKABLE QString labelFor(int i) const { return Menu::label(i); }
  Q_INVOKABLE QString iconFor(int i) const { return Menu::icon(i); }
  Q_INVOKABLE QString qmlFor(int i) const { return Menu::qml(i); }

  Q_INVOKABLE QVariantList getRow1Items() const;
  Q_INVOKABLE QVariantList getRow2Items() const;
  Q_INVOKABLE QVariantList getChildrenOf(int parent) const;

public slots:
  void setSelectedIndex(int i);
  void activate();
  void goHome();
  void moveNext();
  void movePrev();

  void enterButtonPressed() override;
  void downButtonPressed() override;
  void upButtonPressed() override;
  void homeButtonPressed() override;
  void buttonUpdate() override;

signals:
  void selectionChanged();
  void activePageChanged();
  void expandedChanged();
  void isTsOnChanged(bool);
  void themeChanged(const QString &theme);
  void exitRequested();

private:
  void setActivePage(int i);
  void setExpanded(int i);
  void expand(int i);
  void collapse();
  void executeAction(int i);
  void rebuildNavOrder();
  QVariantMap buildItem(int i) const;

  int m_selected = 0;
  int m_activePage = -1;
  int m_expanded = -1;
  bool m_isTsOn = false;
  QVector<int> m_navOrder;
};

#endif // NAVIGATIONCONTROLLER_H
