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

void NavigationController::downButtonPressed() {
  if (this->m_selectedPageIndex + 1 < this->m_numPages) {
    this->setSelectedPageIndex(this->m_selectedPageIndex + 1);
  }
}

void NavigationController::upButtonPressed() {
  if (this->m_selectedPageIndex - 1 >= 0) {
    this->setSelectedPageIndex(this->m_selectedPageIndex - 1);
  }
}

void NavigationController::enterButtonPressed() {
  if (this->m_selectedPageIndex == this->m_numPages - 1) {
    exit(0);
  }
  this->m_model->currentPageIndex = this->m_selectedPageIndex;
  this->setIsSelected(true);
}

void NavigationController::homeButtonPressed() {
  this->m_model->currentPageIndex = -1;
  this->setIsSelected(false);
}

void NavigationController::buttonUpdate() {
  if (this->m_model->currentPageIndex == this->m_pageIndex) {
    std::optional<float> modeIndex = this->m_model->getModeIndex();
    qDebug() << "Mode Index: " << modeIndex.value_or(-999);
    if (modeIndex) {
      this->setSelectedPageIndex(*modeIndex);
    }

    std::optional<bool> homeButtonPressed =
        this->m_model->getHomeButtonPressed();

    if (homeButtonPressed) {
      if (homeButtonPressed == 0) {
        this->enterButtonPressed();
      } else {
        this->homeButtonPressed();
      }
    } else {
      this->homeButtonPressed();
    }
  }
}
