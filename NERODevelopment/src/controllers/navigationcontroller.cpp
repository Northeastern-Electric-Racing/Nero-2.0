#include "navigationcontroller.h"
#include "../utils/data_type_names.h"

namespace Menu {

const std::vector<Item> &getPages() {
  static const std::vector<Item> pages = {
      {"OFF", Type::Page, "zzz.png", "OffScreen2.qml", nullptr},
      {"PIT - DRIVE", Type::Page, "flag.png", "Pit.qml", nullptr},
      {"PIT - REVERSE", Type::Page, "reverse.png", "Pit.qml", nullptr},
      {"PERFORMANCE", Type::Page, "hare.png", "SpeedMode.qml", nullptr},
      {"ENDURANCE", Type::Page, "turtle.png", "EnduranceScreen.qml", nullptr},
      {"GAMES", Type::Category, "game.png", nullptr, nullptr},
      {"FLAPPY BIRD", Type::SubPage, nullptr, "FlappyBird.qml", nullptr},
      {"SNAKE", Type::SubPage, nullptr, "Snake.qml", nullptr},
      {"2048", Type::SubPage, nullptr, "Game2048.qml", nullptr},
      {"DOOM", Type::SubPage, nullptr, "DoomView.qml", nullptr},
      {"THEMES", Type::Category, "themes.png", nullptr, nullptr},
      {"LIGHT", Type::SubAction, nullptr, nullptr,
       [](NavigationController *c) {
         emit c->themeChanged("light");
         c->collapse();
       }},
      {"DARK", Type::SubAction, nullptr, nullptr,
       [](NavigationController *c) {
         emit c->themeChanged("dark");
         c->collapse();
       }},
      {"WHEEL LED", Type::Category, "led.png", nullptr, nullptr},
      {"RGB", Type::SubAction, nullptr, nullptr,
       [](NavigationController *c) { c->sendLedMode({1.0f}); }},
      {"SPEED", Type::SubAction, nullptr, nullptr,
       [](NavigationController *c) { c->sendLedMode({2.0f, 1.0f, 1.0f}); }},
      {"TEMP", Type::SubAction, nullptr, nullptr,
       [](NavigationController *c) { c->sendLedMode({2.0f, 0.0f, 0.0f}); }},
      {"EXIT", Type::Action, "exit.png", nullptr,
       [](NavigationController *c) { emit c->exitRequested(); }},
  };
  return pages;
}

} // namespace Menu

NavigationController::NavigationController(Model *model, QObject *parent)
    : ButtonController(model, -1, parent) {
  connect(m_model, &Model::onCurrentDataChange, this, [this]() {
    std::optional<int> val = m_model->getById(TSMS);
    if (val) {
      bool newVal = (*val != 0);
      if (m_isTsOn != newVal) {
        m_isTsOn = newVal;
        emit isTsOnChanged(m_isTsOn);
      }
    }
  });
  rebuildNavOrder();
}

void NavigationController::setSelectedIndex(int i) {
  if (m_selected != i) {
    m_selected = i;
    emit selectionChanged();
  }
}

void NavigationController::setActivePage(int i) {
  if (m_activePage != i) {
    m_activePage = i;
    m_model->currentPageIndex = i;
    emit activePageChanged();
  }
}

void NavigationController::setExpanded(int i) {
  if (m_expanded != i) {
    m_expanded = i;
    rebuildNavOrder();
    emit expandedChanged();
  }
}

void NavigationController::rebuildNavOrder() {
  m_navOrder.clear();

  for (int i = 0; i < Menu::count(); ++i) {
    if (!Menu::isTopLevel(i))
      continue;

    if (i == m_expanded && Menu::hasChildren(i)) {
      for (int c : Menu::childrenOf(i)) {
        m_navOrder.append(c);
      }
    } else {
      m_navOrder.append(i);
    }
  }
}

QVariantMap NavigationController::buildItem(int i) const {
  return {{"index", i},
          {"label", Menu::label(i)},
          {"icon", Menu::icon(i)},
          {"hasChildren", Menu::hasChildren(i)}};
}

QVariantList NavigationController::getTopLevelItems() const {
  QVariantList list;
  for (int i : Menu::topLevelIndices()) {
    list.append(buildItem(i));
  }
  return list;
}

QVariantList NavigationController::getChildrenOf(int parent) const {
  QVariantList list;
  for (int i : Menu::childrenOf(parent)) {
    list.append(buildItem(i));
  }
  return list;
}

void NavigationController::moveNext() {
  int pos = m_navOrder.indexOf(m_selected);
  if (pos + 1 < m_navOrder.size()) {
    int next = m_navOrder[pos + 1];
    if (m_expanded >= 0 && Menu::isSubItem(m_selected) &&
        !Menu::isSubItem(next)) {
      collapse();
      return;
    }
    setSelectedIndex(next);
  }
}

void NavigationController::movePrev() {
  int pos = m_navOrder.indexOf(m_selected);
  if (pos > 0) {
    int prev = m_navOrder[pos - 1];
    if (m_expanded >= 0 && Menu::isSubItem(m_selected) &&
        !Menu::isSubItem(prev)) {
      collapse();
      return;
    }
    setSelectedIndex(prev);
  }
}

void NavigationController::activate() {
  if (!Menu::valid(m_selected))
    return;

  Menu::Type type = Menu::get(m_selected).type;

  switch (type) {
  case Menu::Type::Page:
  case Menu::Type::SubPage:
    setActivePage(m_selected);
    break;

  case Menu::Type::Category:
    if (m_expanded == m_selected) {
      collapse();
    } else {
      expand(m_selected);
    }
    break;

  case Menu::Type::Action:
  case Menu::Type::SubAction:
    executeAction(m_selected);
    break;
  }
}

void NavigationController::goHome() {
  setActivePage(-1);
  if (m_expanded >= 0) {
    collapse();
  }
}

void NavigationController::expand(int i) {
  setExpanded(i);

  int firstChild = Menu::firstChildOf(i);
  if (firstChild >= 0) {
    setSelectedIndex(firstChild);
  }
}

void NavigationController::sendLedMode(QList<float> values) {
  m_model->sendMessage(LEDMODE, values);
  qDebug() << "LED mode sent:" << values;
  collapse();
}

void NavigationController::collapse() {
  int wasExpanded = m_expanded;
  setExpanded(-1);

  if (wasExpanded >= 0) {
    setSelectedIndex(wasExpanded);
  }
}

void NavigationController::executeAction(int i) {
  const auto &action = Menu::get(i).action;
  if (action) {
    action(this);
  }
}

void NavigationController::enterButtonPressed() { activate(); }
void NavigationController::downButtonPressed() { moveNext(); }
void NavigationController::upButtonPressed() { movePrev(); }
void NavigationController::leftButtonPressed() { movePrev(); }
void NavigationController::rightButtonPressed() { moveNext(); }
void NavigationController::homeButtonPressed() { goHome(); }

void NavigationController::buttonUpdate() {
  if (!m_pageIndices.contains(m_model->currentPageIndex))
    return;

  std::optional<bool> home = m_model->getHomeButtonPressed();
  if (!home.has_value())
    return;

  if (*home) {
    homeButtonPressed();
    std::optional<int> mode = m_model->getModeIndex();
    if (mode.has_value() && *mode >= 0 && *mode < m_navOrder.size()) {
      setSelectedIndex(m_navOrder[*mode]);
    }
    return;
  }

  if (!isPageActive()) {
    if (m_model->getEnterButtonPressed() == true)
      enterButtonPressed();
    if (m_model->getDownButtonPressed() == true)
      downButtonPressed();
    if (m_model->getUpButtonPressed() == true)
      upButtonPressed();
    if (m_model->getBackwardButtonPressed() == true)
      leftButtonPressed();
    if (m_model->getRightButtonPressed() == true)
      rightButtonPressed();
  }
}
