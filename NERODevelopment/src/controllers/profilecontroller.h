#ifndef PROFILECONTROLLER_H
#define PROFILECONTROLLER_H

#include "buttoncontroller.h"
#include <QMap>
#include <QObject>

class ProfileController : public ButtonController {
  Q_OBJECT
  Q_PROPERTY(QStringList profiles READ profiles CONSTANT)
  Q_PROPERTY(
      QString currentProfile READ currentProfile NOTIFY currentProfileChanged)
  Q_PROPERTY(int highlightedIndex READ highlightedIndex NOTIFY
                 highlightedIndexChanged)
  Q_PROPERTY(int imageIndex READ imageIndex NOTIFY imageIndexChanged)
  Q_PROPERTY(QStringList previewImages READ previewImages NOTIFY
                 highlightedIndexChanged)

public:
  explicit ProfileController(Model *model, QObject *parent = nullptr);

  QStringList profiles() const { return m_profiles; }
  QString currentProfile() const { return m_currentProfile; }
  int highlightedIndex() const { return m_highlighted; }
  int imageIndex() const { return m_imageIndex; }
  // Static screenshots for the highlighted profile, empty if it has none yet
  QStringList previewImages() const;

  // Highlight the current profile on its first image, called when the preview
  // page opens
  Q_INVOKABLE void reset();

public slots:
  void enterButtonPressed() override;
  void leftButtonPressed() override;
  void rightButtonPressed() override;
  void upButtonPressed() override;
  void downButtonPressed() override;

signals:
  void currentProfileChanged();
  void highlightedIndexChanged();
  void imageIndexChanged();

private:
  void setHighlightedIndex(int i);
  void setImageIndex(int i);

  const QStringList m_profiles = {"SOFTWARE", "HARDWARE", "DRIVER"};
  QString m_currentProfile = "SOFTWARE";
  const QMap<QString, QStringList> m_previewImages = {
      {"SOFTWARE",
       {"/qt/qml/content/images/previews/software-home.png",
        "/qt/qml/content/images/previews/software-off.png",
        "/qt/qml/content/images/previews/software-pit.png"}},
      {"HARDWARE",
       {"/qt/qml/content/images/previews/hardware-off.png",
        "/qt/qml/content/images/previews/hardware-endurance.png",
        "/qt/qml/content/images/previews/hardware-performance.png"}},
      {"DRIVER",
       {"/qt/qml/content/images/previews/driver-pit.png",
        "/qt/qml/content/images/previews/driver-performance.png",
        "/qt/qml/content/images/previews/driver-endurance.png"}}};
  int m_highlighted = 0;
  int m_imageIndex = 0;
};

#endif // PROFILECONTROLLER_H
