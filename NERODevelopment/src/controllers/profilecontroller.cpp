#include "profilecontroller.h"

ProfileController::ProfileController(Model *model,
                                     NavigationController *navigation,
                                     QObject *parent)
    : ButtonController{model, Menu::indexOf("PROFILE"), parent},
      m_navigation(navigation) {}

QStringList ProfileController::previewImages() const {
  return m_previewImages.value(m_profiles[m_highlighted]);
}

void ProfileController::reset() {
  setHighlightedIndex(
      std::max(0, static_cast<int>(
                      m_profiles.indexOf(m_navigation->currentProfile()))));
  setImageIndex(0);
}

void ProfileController::setHighlightedIndex(int i) {
  if (m_highlighted != i) {
    m_highlighted = i;
    emit highlightedIndexChanged();
  }
}

void ProfileController::setImageIndex(int i) {
  if (m_imageIndex != i) {
    m_imageIndex = i;
    emit imageIndexChanged();
  }
}

// Up and down switch profiles, starting each one on its first image
void ProfileController::upButtonPressed() {
  setHighlightedIndex(std::max(0, m_highlighted - 1));
  setImageIndex(0);
}

void ProfileController::downButtonPressed() {
  setHighlightedIndex(
      std::min(static_cast<int>(m_profiles.size()) - 1, m_highlighted + 1));
  setImageIndex(0);
}

// Left and right scroll the highlighted profile's preview images
void ProfileController::leftButtonPressed() {
  setImageIndex(std::max(0, m_imageIndex - 1));
}

void ProfileController::rightButtonPressed() {
  int last = static_cast<int>(previewImages().size()) - 1;
  setImageIndex(std::max(0, std::min(last, m_imageIndex + 1)));
}

void ProfileController::enterButtonPressed() {
  m_navigation->setProfile(m_profiles[m_highlighted]);
  m_navigation->goHome();
}
