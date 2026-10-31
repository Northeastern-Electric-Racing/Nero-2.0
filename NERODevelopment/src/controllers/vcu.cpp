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
    const std::optional<int> navIndex = carStateWireValue(value);
    if (navIndex && navIndex != m_navIndex) {
      const bool first = !m_navIndex;
      m_navIndex = navIndex;
      emit navIndexChanged();
      if (first)
        emit homeMenuChanged();
    }
    // A frame's nero_index follows its home_mode, so the slot is known now
    if (navIndex.has_value() && m_opening) {
      m_opening = false;
      emit opened();
    }
  } else if (topic == HOMEBUTTON) {
    const bool homeMenu = value == 1;
    if (homeMenu != m_homeMenu) {
      // Only leaving a home menu the VCU reported opens a slot; the first
      // home_mode heard is just its current state
      m_opening = !homeMenu && m_homeMenu.has_value();
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
