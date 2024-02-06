#ifndef HEADERCONTROLLER_H
#define HEADERCONTROLLER_H

#include "../models/model.h"
#include <QObject>

/**
 * @brief The HeaderController class
 * Controller for the views.
 */
class HeaderController : public QObject {
  Q_OBJECT
  Q_PROPERTY(int numCriticalWarnings READ numCriticalWarnings WRITE
                 setNumCriticalWarnings NOTIFY numCriticalWarningsChanged FINAL)
  Q_PROPERTY(
      int numNonCriticalWarnings READ numNonCriticalWarnings WRITE
          setNumNonCriticalWarnings NOTIFY numNonCriticalWarningsChanged FINAL)
  Q_PROPERTY(bool isTalking READ isTalking WRITE setIsTalking NOTIFY
                 isTalkingChanged FINAL)

public:
  explicit HeaderController(Model *model, QObject *parent = nullptr);
  int numCriticalWarnings() const;
  int numNonCriticalWarnings() const;
  bool isTalking() const;

signals:
  void numCriticalWarningsChanged(int);
  void numNonCriticalWarningsChanged(int);
  void isTalkingChanged(bool);

public slots:
  void setNumCriticalWarnings(int);
  void setNumNonCriticalWarnings(int);
  void setIsTalking(bool);
  void currentDataDidChange();

private:
  Model *m_model;
  int m_numCriticalWarnings;
  int m_numNonCriticalWarnings;
  bool m_isTalking;
};

#endif // HEADERCONTROLLER_H
