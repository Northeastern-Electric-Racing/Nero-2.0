#ifndef NAVIGATIONCONTROLLER_H
#define NAVIGATIONCONTROLLER_H

#include "../constants/vcu_state_labels.h"
#include "../controllers/buttoncontroller.h"
#include "../models/model.h"

class NavigationController;

namespace Menu {
Q_NAMESPACE

enum class Type { Page, Category, Action, SubPage, SubAction };
Q_ENUM_NS(Type)

using ActionFunc = std::function<void(NavigationController *)>;

struct Item {
  const char *label;
  Type type;
  const char *icon;
  const char *qml;
  ActionFunc action;
};

const std::vector<Item> &getPages();

inline int count() { return static_cast<int>(getPages().size()); }
inline bool valid(int i) { return i >= 0 && i < count(); }
inline const Item &get(int i) { return getPages()[i]; }

inline bool isSubItem(int i) {
  return valid(i) &&
         (get(i).type == Type::SubPage || get(i).type == Type::SubAction);
}

inline bool isTopLevel(int i) { return valid(i) && !isSubItem(i); }

inline bool isCategory(int i) {
  return valid(i) && get(i).type == Type::Category;
}

inline int firstChildOf(int parent) {
  if (!valid(parent) || !isCategory(parent))
    return -1;
  int next = parent + 1;
  return (valid(next) && isSubItem(next)) ? next : -1;
}

inline bool hasChildren(int i) { return firstChildOf(i) >= 0; }

inline QString label(int i) {
  return valid(i) && get(i).label ? QString::fromUtf8(get(i).label) : QString();
}

inline QString icon(int i) {
  if (!valid(i) || !get(i).icon)
    return QString();
  return QStringLiteral("/qt/qml/content/images/%1")
      .arg(QString::fromUtf8(get(i).icon));
}

inline QString qml(int i) {
  return valid(i) && get(i).qml ? QString::fromUtf8(get(i).qml) : QString();
}

inline QVector<int> topLevelIndices() {
  QVector<int> r;
  for (int i = 0; i < count(); ++i)
    if (isTopLevel(i))
      r.append(i);
  return r;
}

inline QVector<int> childrenOf(int parent) {
  QVector<int> r;
  if (!isCategory(parent))
    return r;
  for (int i = parent + 1; i < count() && isSubItem(i); ++i)
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
  Q_PROPERTY(QString functionalStateLabel READ functionalStateLabel NOTIFY
                 functionalStateChanged)
  Q_PROPERTY(bool functionalStateKnown READ functionalStateKnown NOTIFY
                 functionalStateChanged)
  Q_PROPERTY(bool functionalStateFaulted READ functionalStateFaulted NOTIFY
                 functionalStateChanged)
  Q_PROPERTY(QList<QString> stateRejectionReasons READ stateRejectionReasons
                 NOTIFY stateRejectionChanged)
  Q_PROPERTY(bool stateRejectionActive READ stateRejectionActive NOTIFY
                 stateRejectionChanged)

public:
  explicit NavigationController(Model *model, QObject *parent = nullptr);

  int selectedIndex() const { return m_selected; }
  int activePageIndex() const { return m_activePage; }
  int expandedIndex() const { return m_expanded; }
  bool isPageActive() const { return m_activePage >= 0; }
  bool isTsOn() const { return m_isTsOn; }

  QString functionalStateLabel() const {
    return functionalStateText(m_functionalState);
  }
  bool functionalStateKnown() const {
    return functionalStateIsKnown(m_functionalState);
  }
  bool functionalStateFaulted() const {
    return functionalStateIsFaulted(m_functionalState);
  }
  QList<QString> stateRejectionReasons() const {
    return stateRejectionTexts(m_stateRejection);
  }
  bool stateRejectionActive() const {
    return stateRejectionIsActive(m_stateRejection);
  }

  Q_INVOKABLE QString labelFor(int i) const { return Menu::label(i); }
  Q_INVOKABLE QString iconFor(int i) const { return Menu::icon(i); }
  Q_INVOKABLE QString qmlFor(int i) const { return Menu::qml(i); }

  Q_INVOKABLE QVariantList getTopLevelItems() const;
  Q_INVOKABLE QVariantList getChildrenOf(int parent) const;

  // Jump to a top-level page or HOME by label, dev screenshot harness only
  Q_INVOKABLE bool jumpToPage(const QString &label);

  void collapse();
  void sendLedMode(QList<float> values);

public slots:
  void setSelectedIndex(int i);
  void activate();
  void goHome();
  void moveNext();
  void movePrev();

  void buttonUpdate() override;

signals:
  void selectionChanged();
  void activePageChanged();
  void expandedChanged();
  void isTsOnChanged(bool);
  void functionalStateChanged();
  void stateRejectionChanged();
  void themeChanged(const QString &theme);
  void exitRequested();

private:
  void setActivePage(int i);
  void setExpanded(int i);
  void expand(int i);
  void executeAction(int i);
  void rebuildNavOrder();
  QVariantMap buildItem(int i) const;

  void syncModeFromVcu();

  void updateCarStateReadout();
  void setFunctionalState(std::optional<int> state);
  void setStateRejection(std::optional<int> mask);

  int m_selected = 0;
  int m_activePage = -1;
  int m_expanded = -1;
  bool m_isTsOn = false;
  std::optional<int> m_functionalState;
  std::optional<int> m_stateRejection;
  int m_lastModeIndex = -999;
  bool m_lastHomeMode = false;
  QVector<int> m_navOrder;
};

#endif // NAVIGATIONCONTROLLER_H
