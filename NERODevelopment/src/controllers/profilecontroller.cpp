#include "profilecontroller.h"

ProfileController::ProfileController(Model *model, QObject *parent)
    : ButtonController{model, parent} {}

QStringList ProfileController::previewImages() const {
  return m_previewImages.value(m_profiles[m_highlighted]);
}

void ProfileController::reset() {
  setHighlightedIndex(
      std::max(0, static_cast<int>(m_profiles.indexOf(m_currentProfile))));
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

// Escape goes home through the VCU, so Enter only switches the profile
void ProfileController::enterButtonPressed() {
  if (m_currentProfile != m_profiles[m_highlighted]) {
    m_currentProfile = m_profiles[m_highlighted];
    emit currentProfileChanged();
  }
}
