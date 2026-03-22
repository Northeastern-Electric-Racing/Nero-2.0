#include "navigationcontroller.h"
#include "../utils/data_type_names.h"

namespace Menu {

const std::vector<Item> &getPages() {
    static const std::vector<Item> pages = {
                                            {"OFF", Type::Page, "zzz.png", "OffScreen2.qml", nullptr},
                                            {"PIT - DRIVE", Type::Page, "flag.png", "Pit.qml", nullptr},
                                            {"PIT - REVERSE", Type::Page, "reverse.png", "Pit.qml", nullptr},
                                            {"PERFORMANCE", Type::Page, "hare.png", "SpeedMode.qml", nullptr},
                                            {"EFFICIENCY", Type::Page, "turtle.png", "EfficiencyScreen.qml", nullptr},
                                            {"GAMES", Type::Category, "game.png", nullptr, nullptr},
                                            {"FLAPPY BIRD", Type::SubPage, nullptr, "FlappyBird.qml", nullptr},
                                            {"SNAKE", Type::SubPage, nullptr, "Snake.qml", nullptr},
                                            {"2048", Type::SubPage, nullptr, "Game2048.qml", nullptr},
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
void NavigationController::homeButtonPressed() { goHome(); }

void NavigationController::buttonUpdate() {
    // Home/esc ALWAYS works from any page, MQTT only
    if (!m_model->lastButtonFromSocket) {
        std::optional<float> homeValue = m_model->getById(HOMEBUTTON);
        if (homeValue.has_value() && homeValue.value() == 1) {
            m_model->setValue(HOMEBUTTON, 10);
            homeButtonPressed();
            std::optional<float> mode = m_model->getModeIndex();
            if (mode.has_value() && *mode >= 0 && *mode < m_navOrder.size()) {
                setSelectedIndex(m_navOrder[static_cast<int>(*mode)]);
            }
            return;
        }
    }

    // Navigation buttons only on home screen (page -1)
    if (!m_pageIndices.contains(m_model->currentPageIndex))
        return;

    // On home screen: MQTT only — ignore socket buttons
    if (m_model->lastButtonFromSocket)
        return;

    // Directional/action buttons — read ONCE, dispatch by value
    if (!isPageActive()) {
        std::optional<float> buttonValue = m_model->getById(ENTERBUTTON);
        if (buttonValue.has_value()) {
            float val = buttonValue.value();

            if (val == 5) {
                m_model->setValue(ENTERBUTTON, 10);
                enterButtonPressed();
            } else if (val == 3 || val == 1) {
                // down or right = move next (forward)
                m_model->setValue(ENTERBUTTON, 10);
                downButtonPressed();
            } else if (val == 4 || val == 0) {
                // up or left = move prev (backward)
                m_model->setValue(ENTERBUTTON, 10);
                upButtonPressed();
            }
        }
    }
}
