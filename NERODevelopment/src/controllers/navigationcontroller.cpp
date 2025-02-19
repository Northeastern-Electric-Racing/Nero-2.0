#include "navigationcontroller.h"

NavigationController::NavigationController(Model *model, QObject *parent)
    : ButtonController{model, -1, parent} {}

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

void NavigationController::downButtonPressed() {
  if (m_gamesSelected) {
    if (this->m_selectedPageIndex + 2 < this->m_numGames + this->m_numPages) {
      this->setSelectedPageIndex(this->m_selectedPageIndex + 1);
    }
  } else {
    if (this->m_selectedPageIndex + 1 < this->m_numPages) {
      this->setSelectedPageIndex(this->m_selectedPageIndex + 1);
    }
  }
}

void NavigationController::upButtonPressed() {
  if (m_gamesSelected) {
    if (this->m_selectedPageIndex >= this->m_numPages) {
      this->setSelectedPageIndex(this->m_selectedPageIndex - 1);
    }
  } else {
    if (this->m_selectedPageIndex - 1 >= 0) {
      this->setSelectedPageIndex(this->m_selectedPageIndex - 1);
    }
  }
}

void NavigationController::enterButtonPressed() {
  if (!this->m_gamesSelected &&
      this->m_selectedPageIndex == this->m_numPages - 1) {
    this->exitProgram();
  } else if (this->m_selectedPageIndex == this->m_numPages - 2 &&
             !this->m_gamesSelected) {
    this->setIsGamesOpen(true);
    this->setSelectedPageIndex(this->m_numPages - 1);
  } else {
    this->m_model->currentPageIndex = this->m_selectedPageIndex;
    this->setIsSelected(true);
  }
}

void NavigationController::homeButtonPressed() {
  if (m_gamesSelected) {
    this->setIsGamesOpen(false);
    this->setSelectedPageIndex(this->m_numPages - 1);
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
    } else if (!this->m_gamesSelected) {
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
