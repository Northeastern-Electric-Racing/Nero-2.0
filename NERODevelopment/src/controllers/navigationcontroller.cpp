#include "navigationcontroller.h"
#include "../utils/data_type_names.h"

NavigationController::NavigationController(Model *model, QObject *parent)
    : ButtonController(model, -1, parent) {
  connect(m_model, &Model::onCurrentDataChange, this, [this]() {
    auto val = m_model->getById(TSMS);
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

  for (int i = 0; i < Menu::COUNT; ++i) {
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

QVariantList NavigationController::getRow1Items() const {
  QVariantList list;
  for (int i : Menu::topLevelInRow(1)) {
    list.append(buildItem(i));
  }
  return list;
}

QVariantList NavigationController::getRow2Items() const {
  QVariantList list;
  for (int i : Menu::topLevelInRow(2)) {
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
    setSelectedIndex(m_navOrder[pos + 1]);
  }
}

void NavigationController::movePrev() {
  int pos = m_navOrder.indexOf(m_selected);
  if (pos > 0) {
    setSelectedIndex(m_navOrder[pos - 1]);
  }
}

void NavigationController::activate() {
  if (!Menu::valid(m_selected))
    return;

  Menu::Type type = Menu::PAGES[m_selected].type;

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

void NavigationController::collapse() {
  int wasExpanded = m_expanded;
  setExpanded(-1);

  if (wasExpanded >= 0) {
    setSelectedIndex(wasExpanded);
  }
}

void NavigationController::executeAction(int i) {
  QString lbl = Menu::label(i);

  if (lbl == "EXIT") {
    emit exitRequested();
  } else if (lbl == "LIGHT") {
    emit themeChanged("light");
    collapse();
  } else if (lbl == "DARK") {
    emit themeChanged("dark");
    collapse();
  }
}

void NavigationController::enterButtonPressed() { activate(); }
void NavigationController::downButtonPressed() { moveNext(); }
void NavigationController::upButtonPressed() { movePrev(); }
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
  }
}
