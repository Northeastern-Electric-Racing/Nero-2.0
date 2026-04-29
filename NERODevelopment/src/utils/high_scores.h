#ifndef HIGH_SCORES_H
#define HIGH_SCORES_H

#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <QString>

inline QString highScoresPath() {
  QString dir = qEnvironmentVariable("NERO_DATA_DIR");
  if (dir.isEmpty())
    dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkpath(dir);
  return dir + "/highscores.ini";
}

inline int loadHighScore(const QString &key) {
  QSettings s(highScoresPath(), QSettings::IniFormat);
  return s.value(key, 0).toInt();
}

inline void saveHighScore(const QString &key, int score) {
  QSettings s(highScoresPath(), QSettings::IniFormat);
  s.setValue(key, score);
  s.sync();
}

#endif // HIGH_SCORES_H
