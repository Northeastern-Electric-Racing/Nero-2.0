#include "navigationcontroller.h"
#include "../utils/data_type_names.h"

NavigationController::NavigationController(Model *model, QObject *parent)
    : ButtonController{model, -1, parent}, m_isTsOn(false) {
  connect(m_model, &Model::onCurrentDataChange, this,
          &NavigationController::currentDataDidChange);
}

void NavigationController::currentDataDidChange() {
  if (this->m_pageIndices.contains(this->m_model->currentPageIndex)) {
    std::optional<float> value = this->m_model->getById(TSMS);
    if (value) {
      if (value == 0) {
        setIsTsOn(false);
      } else {
        setIsTsOn(true);
      }
    }
  }
}

bool NavigationController::isTsOn() const { return m_isTsOn; }

void NavigationController::setIsTsOn(bool isTsOn) {
  if (m_isTsOn != isTsOn) {
    m_isTsOn = isTsOn;
    emit isTsOnChanged(isTsOn);
  }
}

int NavigationController::selectedPageIndex() const {
  return this->m_selectedPageIndex;
}
void NavigationController::setSelectedPageIndex(int index) {
  if (index != this->m_selectedPageIndex) {
    this->m_selectedPageIndex = index;
    emit this->selectedPageIndexChanged();
  }
}

bool NavigationController::isSelected() const { return this->m_isSelected; }
void NavigationController::setIsSelected(bool isSelected) {
  if (isSelected != this->m_isSelected) {
    this->m_isSelected = isSelected;
    emit this->isSelectedChanged();
  }
}

bool NavigationController::isGamesOpen() const { return this->m_gamesSelected; }
void NavigationController::setIsGamesOpen(bool isGamesOpen) {
  if (isGamesOpen != this->m_gamesSelected) {
    this->m_gamesSelected = isGamesOpen;
    emit this->isGamesOpenChanged();
  }
}

bool NavigationController::isThemesOpen() const { return m_themesSelected; }
void NavigationController::setIsThemesOpen(bool open) {
  if (m_themesSelected != open) {
    m_themesSelected = open;
    emit isThemesOpenChanged();
  }
}

void NavigationController::downButtonPressed() {
  if (m_gamesSelected) {
    if (this->m_selectedPageIndex < this->m_numPages + 1) {
      this->setSelectedPageIndex(this->m_selectedPageIndex + 1);
    }
  } else if (m_themesSelected) {
    if (this->m_selectedPageIndex < this->m_numPages) {
      this->setSelectedPageIndex(this->m_selectedPageIndex + 1);
    }
  } else {
    if (this->m_selectedPageIndex + 1 < this->m_numPages) {
      this->setSelectedPageIndex(this->m_selectedPageIndex + 1);
    }
  }
}

void NavigationController::upButtonPressed() {
  qDebug() << "up";
  if (m_gamesSelected) {
    if (this->m_selectedPageIndex > this->m_numPages - 2) {
      this->setSelectedPageIndex(this->m_selectedPageIndex - 1);
    }
  } else if (m_themesSelected) {
    if (this->m_selectedPageIndex > this->m_numPages - 2) {
      this->setSelectedPageIndex(this->m_selectedPageIndex - 1);
    }
  } else {
    if (this->m_selectedPageIndex - 1 >= 0) {
      this->setSelectedPageIndex(this->m_selectedPageIndex - 1);
    }
  }
}

void NavigationController::enterButtonPressed() {
  if ((m_gamesSelected && m_selectedPageIndex == m_numPages + 1) ||
      (m_themesSelected && m_selectedPageIndex == m_numPages) ||
      (!m_gamesSelected && !m_themesSelected &&
       m_selectedPageIndex == m_numPages - 1)) {
    exitProgram();
    return;
  }

  if (!m_gamesSelected && !m_themesSelected) {
    if (m_selectedPageIndex == m_numPages - 3) {
      setIsGamesOpen(true);
      setSelectedPageIndex(m_numPages - 2);
      return;
    }
    if (m_selectedPageIndex == m_numPages - 2) {
      setIsThemesOpen(true);
      setSelectedPageIndex(m_numPages - 2);
      return;
    }
    m_model->currentPageIndex = m_selectedPageIndex;
    setIsSelected(true);
    return;
  }

  if (m_gamesSelected) {
    if (m_selectedPageIndex == m_numPages - 2 ||
        m_selectedPageIndex == m_numPages - 1) {
      m_model->currentPageIndex = m_selectedPageIndex;
      setIsSelected(true);
      return;
    }
    if (m_selectedPageIndex == m_numPages) {
      setIsGamesOpen(false);
      setIsThemesOpen(true);
      setSelectedPageIndex(m_numPages - 2);
      return;
    }
  }

  if (m_themesSelected) {
    if (m_selectedPageIndex == m_numPages - 2 ||
        m_selectedPageIndex == m_numPages - 1) {
      QString theme =
          (m_selectedPageIndex == m_numPages - 2) ? "light" : "dark";
      emit themeChanged(theme);
      setIsThemesOpen(false);
      setSelectedPageIndex(m_numPages - 2);
      return;
    }
    if (m_selectedPageIndex == m_numPages - 3) {
      setIsThemesOpen(false);
      setIsGamesOpen(true);
      setSelectedPageIndex(m_numPages - 2);
      return;
    }
  }
}

void NavigationController::homeButtonPressed() {
  if (m_gamesSelected) {
    this->setIsGamesOpen(false);
    this->setSelectedPageIndex(this->m_numPages - 3);
  }
  if (m_themesSelected) {
    this->setIsThemesOpen(false);
    this->setSelectedPageIndex(this->m_numPages - 2);
  }
  this->m_model->currentPageIndex = -1;
  this->setIsSelected(false);
}

void NavigationController::exitProgram() { exit(0); }

void NavigationController::buttonUpdate() {
  if (this->m_pageIndices.contains(this->m_model->currentPageIndex)) {
    std::optional<float> modeIndex = this->m_model->getModeIndex();
    // qDebug() << "Mode Index: " << modeIndex.value_or(-999);

    std::optional<bool> homeButtonPressed =
        this->m_model->getHomeButtonPressed();

    // qDebug() << "Home button: " << homeButtonPressed.value_or(-999);

    std::optional<bool> enterButtonPressed =
        this->m_model->getEnterButtonPressed();
    std::optional<bool> downButtonPressed =
        this->m_model->getDownButtonPressed();
    std::optional<bool> upButtonPressed = this->m_model->getUpButtonPressed();

    if (!homeButtonPressed.has_value()) {
      return;
    }

    if (homeButtonPressed == true) {
      this->homeButtonPressed();
      if (modeIndex) {
        this->setSelectedPageIndex(*modeIndex);
      }
    } else if (!this->m_gamesSelected && !this->m_themesSelected) {
      this->enterButtonPressed();
    } else {
      if (enterButtonPressed == true) {
        this->enterButtonPressed();
      }
      if (downButtonPressed) {
        this->downButtonPressed();
      }
      if (upButtonPressed) {
        this->upButtonPressed();
      }
    }
  }
}
