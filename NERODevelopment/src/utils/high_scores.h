#ifndef HIGH_SCORES_H
#define HIGH_SCORES_H

#include <QSettings>
#include <QString>

namespace HighScores {

inline QSettings settings() {
  const QString dir = qEnvironmentVariable("NERO_DATA_DIR");
  return dir.isEmpty()
             ? QSettings()
             : QSettings(dir + "/highscores.ini", QSettings::IniFormat);
}

inline int load(const QString &key) { return settings().value(key, 0).toInt(); }

inline void save(const QString &key, int score) {
  QSettings s = settings();
  s.setValue(key, score);
  s.sync();
}

} // namespace HighScores

#endif // HIGH_SCORES_H
