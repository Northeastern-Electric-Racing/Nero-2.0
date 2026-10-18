#include "vcu.h"
#include "../utils/data_type_names.h"

// The VCU sends car state every 200 ms; five missed messages means it's gone
static constexpr int LOST_AFTER_MS = 1000;

Vcu::Vcu(Model *model, QObject *parent) : QObject{parent} {
  m_lostTimer.setSingleShot(true);
  m_lostTimer.setInterval(LOST_AFTER_MS);
  connect(&m_lostTimer, &QTimer::timeout, this, [this] {
    setFunctionalState(std::nullopt);
    setStateRejection(std::nullopt);
  });
  connect(model, &Model::topicReceived, this, &Vcu::onTopic);
}

void Vcu::onTopic(const QString &topic, float value) {
  if (topic == MODEINDEX) {
    const int navIndex = carStateWireValue(value).value_or(m_navIndex);
    if (navIndex != m_navIndex) {
      m_navIndex = navIndex;
      emit navIndexChanged();
    }
  } else if (topic == HOMEBUTTON) {
    const bool homeMenu = value == 1;
    if (homeMenu != m_homeMenu) {
      m_homeMenu = homeMenu;
      emit homeMenuChanged();
    }
  } else if (topic == TSMS) {
    const bool tsOn = value != 0;
    if (tsOn != m_tsOn) {
      m_tsOn = tsOn;
      emit tsOnChanged();
    }
  } else if (topic == FUNCTIONALSTATE) {
    m_lostTimer.start();
    setFunctionalState(carStateWireValue(value));
  } else if (topic == STATEREJECTIONERROR && m_functionalState) {
    // Only while the VCU is heard, so a stale mask can't show under NO VCU
    setStateRejection(carStateWireValue(value));
  }
}

void Vcu::setFunctionalState(std::optional<int> state) {
  if (state != m_functionalState) {
    m_functionalState = state;
    emit functionalStateChanged();
  }
}

void Vcu::setStateRejection(std::optional<int> mask) {
  if (mask != m_stateRejection) {
    m_stateRejection = mask;
    emit stateRejectionChanged();
  }
}
