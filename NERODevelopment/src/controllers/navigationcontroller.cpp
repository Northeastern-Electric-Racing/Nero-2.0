#include "navigationcontroller.h"

NavigationController::NavigationController(Model *model, QObject *parent)
    : ButtonController{model, parent} {}

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
    qDebug() << "Setting isSelected to " << isSelected << " from "
             << this->m_isSelected
             << " in NavigationController::setIsSelected(bool isSelected)";
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
  this->setIsSelected(true);
}

void NavigationController::homeButtonPressed() { this->setIsSelected(false); }
