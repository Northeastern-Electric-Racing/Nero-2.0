#ifndef HOMECONTROLLER_H
#define HOMECONTROLLER_H

#include <QObject>

class HomeController : public QObject {
  Q_OBJECT
public:
  explicit HomeController(QObject *parent = nullptr);

signals:

public slots:
  void callMe();
};

#endif // HOMECONTROLLER_H
