#include "navigationcontroller.h"
#include "../utils/data_type_names.h"

namespace Menu {

const std::vector<Item> &getPages() {
  static const std::vector<Item> pages = {
      {"OFF", Type::Page, "zzz.png", "OffScreen2.qml", nullptr},
      {"PIT - DRIVE", Type::Page, "flag.png", "Pit.qml", nullptr},
      {"PIT - KEVIN", Type::Page, "kevin.jpg", "Pit.qml", nullptr},
      {"PERFORMANCE", Type::Page, "hare.png", "SpeedMode.qml", nullptr},
      {"ENDURANCE", Type::Page, "turtle.png", "EnduranceScreen.qml", nullptr},
      {"GAMES", Type::Category, "game.png", nullptr, nullptr},
      {"FLAPPY BIRD", Type::SubPage, nullptr, "FlappyBird.qml", nullptr},
      {"SNAKE", Type::SubPage, nullptr, "Snake.qml", nullptr},
      {"2048", Type::SubPage, nullptr, "Game2048.qml", nullptr},
      {"DOOM", Type::SubPage, nullptr, "DoomView.qml", nullptr},
      {"THEMES", Type::Category, "themes.png", nullptr, nullptr},
      {"LIGHT", Type::SubAction, nullptr, nullptr,
       [](NavigationController *c) { emit c->themeChanged("light"); }},
      {"DARK", Type::SubAction, nullptr, nullptr,
       [](NavigationController *c) { emit c->themeChanged("dark"); }},
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
  connect(m_model, &Model::onCurrentDataChange, this,
          &NavigationController::syncModeFromVcu);
  connect(m_model, &Model::onCurrentDataChange, this,
          &NavigationController::updateCarStateReadout);
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

// Dev testing only for now, drives the screenshot harness
bool NavigationController::jumpToPage(const QString &label) {
  // HOME is the menu screen, not a menu entry, so it needs goHome
  if (label.compare("HOME", Qt::CaseInsensitive) == 0) {
    goHome();
    return true;
  }
  for (int i : Menu::topLevelIndices()) {
    if (Menu::label(i).compare(label, Qt::CaseInsensitive) == 0) {
      setSelectedIndex(i);
      activate();
      return true;
    }
  }
  return false;
}

void NavigationController::moveNext() {
  int pos = m_navOrder.indexOf(m_selected);
  if (pos + 1 < m_navOrder.size()) {
    int next = m_navOrder[pos + 1];
    if (m_expanded >= 0 && Menu::isSubItem(m_selected) &&
        !Menu::isSubItem(next)) {
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

void NavigationController::syncModeFromVcu() {
  std::optional<float> idxRaw = m_model->getModeIndex();
  std::optional<bool> homeRaw = m_model->getHomeButtonPressed();
  if (!idxRaw.has_value() || !homeRaw.has_value())
    return;

  int idx = static_cast<int>(*idxRaw);
  bool home = *homeRaw;
  if (idx == m_lastModeIndex && home == m_lastHomeMode)
    return;
  m_lastModeIndex = idx;
  m_lastHomeMode = home;

  const QVector<int> tops = Menu::topLevelIndices();
  if (idx < 0 || idx >= tops.size())
    return;

  int top = tops[idx];

  if (home) {
    setActivePage(-1);
    setExpanded(-1);
    setSelectedIndex(top);
    return;
  }

  switch (Menu::get(top).type) {
  case Menu::Type::Category:
    setActivePage(-1);
    expand(top);
    break;
  case Menu::Type::Action:
    setActivePage(-1);
    setExpanded(-1);
    setSelectedIndex(top);
    break;
  default:
    setExpanded(-1);
    setSelectedIndex(top);
    setActivePage(top);
    break;
  }
}

void NavigationController::setFunctionalState(std::optional<int> state) {
  if (m_functionalState == state)
    return;
  m_functionalState = state;
  emit functionalStateChanged();
}

void NavigationController::setStateRejection(std::optional<int> mask) {
  if (m_stateRejection == mask)
    return;
  m_stateRejection = mask;
  emit stateRejectionChanged();
}

void NavigationController::updateCarStateReadout() {
  setFunctionalState(carStateWireValue(m_model->getFunctionalState()));
  setStateRejection(carStateWireValue(m_model->getStateRejectionError()));
}

void NavigationController::buttonUpdate() {
  if (!m_pageIndices.contains(m_model->currentPageIndex))
    return;

  bool left = m_model->getLeftButtonPressed() == true;
  bool right = m_model->getRightButtonPressed() == true;
  bool enter = m_model->getEnterButtonPressed() == true;

  if (m_expanded < 0)
    return;

  if (left)
    movePrev();
  if (right)
    moveNext();
  if (enter)
    activate();
}
