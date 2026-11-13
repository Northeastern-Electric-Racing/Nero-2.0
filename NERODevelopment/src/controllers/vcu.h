#ifndef VCU_H
#define VCU_H

#include "../constants/vcu_state_labels.h"
#include "../models/model.h"
#include <QObject>
#include <QTimer>

/**
 * @brief Read-only mirror of the VCU's state. The VCU owns navigation.
 */
class Vcu : public QObject {
  Q_OBJECT
  Q_PROPERTY(int navIndex READ navIndex NOTIFY navIndexChanged)
  Q_PROPERTY(bool homeMenu READ homeMenu NOTIFY homeMenuChanged)
  Q_PROPERTY(bool tsOn READ tsOn NOTIFY tsOnChanged)
  Q_PROPERTY(QString functionalStateLabel READ functionalStateLabel NOTIFY
                 functionalStateChanged)
  Q_PROPERTY(bool functionalStateKnown READ functionalStateKnown NOTIFY
                 functionalStateChanged)
  Q_PROPERTY(bool functionalStateFaulted READ functionalStateFaulted NOTIFY
                 functionalStateChanged)
  Q_PROPERTY(QStringList stateRejectionReasons READ stateRejectionReasons NOTIFY
                 stateRejectionChanged)
  Q_PROPERTY(bool stateRejectionActive READ stateRejectionActive NOTIFY
                 stateRejectionChanged)

public:
  explicit Vcu(Model *model, QObject *parent = nullptr);

  int navIndex() const { return m_navIndex.value_or(0); }
  // The VCU sends home_mode before nero_index, so on the first car state a
  // page would open at the default index; stay on the menu until both arrive
  bool homeMenu() const { return m_homeMenu.value_or(true) || !m_navIndex; }
  bool tsOn() const { return m_tsOn; }
  QString functionalStateLabel() const {
    return functionalStateText(m_functionalState);
  }
  bool functionalStateKnown() const {
    return functionalStateIsKnown(m_functionalState);
  }
  bool functionalStateFaulted() const {
    return functionalStateIsFaulted(m_functionalState);
  }
  QStringList stateRejectionReasons() const {
    return stateRejectionTexts(m_stateRejection);
  }
  bool stateRejectionActive() const {
    return stateRejectionIsActive(m_stateRejection);
  }

signals:
  void navIndexChanged();
  void homeMenuChanged();
  void opened();
  void tsOnChanged();
  void functionalStateChanged();
  void stateRejectionChanged();

private:
  void onTopic(const QString &topic, float value);
  void setFunctionalState(std::optional<int> state);
  void setStateRejection(std::optional<int> mask);

  std::optional<int> m_navIndex;
  std::optional<bool> m_homeMenu;
  bool m_opening = false;
  bool m_tsOn = false;
  std::optional<int> m_functionalState;
  std::optional<int> m_stateRejection;
  QTimer m_lostTimer;
};

#endif // VCU_H
