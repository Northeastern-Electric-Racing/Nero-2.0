#ifndef VCU_STATE_LABELS_H
#define VCU_STATE_LABELS_H

#include <QList>
#include <QString>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <optional>

#define CAR_STATE_ABSENT_LABEL "NO VCU"
#define CAR_STATE_INVALID_LABEL "INVALID"
#define CAR_STATE_UNKNOWN_REASON_LABEL "UNKNOWN REASON"
#define STATE_REJECTION_KNOWN_BITS 0x7F

enum class FunctionalState : int {
  Ready = 0,
  Pit = 1,
  Reverse = 2,
  Performance = 3,
  Efficiency = 4,
  Faulted = 5,
};

enum class StateRejectionError : int {
  ReverseDisabled = 1 << 0,
  DriveFromFault = 1 << 1,
  EnterDriveShutdownOpen = 1 << 2,
  EnterDriveBrakesNotEngaged = 1 << 3,
  EnterGamesShutdownClosed = 1 << 4,
  EnterGamesWhileMoving = 1 << 5,
  ChangeStateAccelPressed = 1 << 6,
};

struct CarStateLabel {
  int value;
  const char *label;
};

inline constexpr std::array<CarStateLabel, 6> FUNCTIONAL_STATE_LABELS = {
    {{static_cast<int>(FunctionalState::Ready), "READY"},
     {static_cast<int>(FunctionalState::Pit), "PIT"},
     {static_cast<int>(FunctionalState::Reverse), "REVERSE"},
     {static_cast<int>(FunctionalState::Performance), "PERFORMANCE"},
     {static_cast<int>(FunctionalState::Efficiency), "EFFICIENCY"},
     {static_cast<int>(FunctionalState::Faulted), "FAULTED"}}};

inline constexpr std::array<CarStateLabel, 7> STATE_REJECTION_LABELS = {
    {{static_cast<int>(StateRejectionError::ReverseDisabled), "NO REV"},
     {static_cast<int>(StateRejectionError::DriveFromFault), "CLR FAULT"},
     {static_cast<int>(StateRejectionError::EnterDriveShutdownOpen), "SD OPEN"},
     {static_cast<int>(StateRejectionError::EnterDriveBrakesNotEngaged),
      "NO BRAKE"},
     {static_cast<int>(StateRejectionError::EnterGamesShutdownClosed),
      "SD CLOSED"},
     {static_cast<int>(StateRejectionError::EnterGamesWhileMoving),
      "CAR MOVING"},
     {static_cast<int>(StateRejectionError::ChangeStateAccelPressed),
      "REL ACCEL"}}};

inline std::optional<int> carStateWireValue(std::optional<float> raw) {
  if (!raw.has_value() || !std::isfinite(*raw))
    return std::nullopt;
  return static_cast<int>(std::clamp(*raw, -1000.0f, 1000.0f));
}

template <std::size_t N>
inline std::optional<QString>
carStateLookup(const std::array<CarStateLabel, N> &table,
               std::optional<int> wire) {
  if (!wire.has_value())
    return std::nullopt;
  for (const CarStateLabel &entry : table) {
    if (entry.value == *wire)
      return QString::fromUtf8(entry.label);
  }
  return std::nullopt;
}

inline QString functionalStateText(std::optional<int> wire) {
  if (!wire.has_value())
    return QStringLiteral(CAR_STATE_ABSENT_LABEL);
  return carStateLookup(FUNCTIONAL_STATE_LABELS, wire)
      .value_or(QStringLiteral(CAR_STATE_INVALID_LABEL));
}

inline bool functionalStateIsKnown(std::optional<int> wire) {
  return carStateLookup(FUNCTIONAL_STATE_LABELS, wire).has_value();
}

inline bool functionalStateIsFaulted(std::optional<int> wire) {
  if (!wire.has_value() || !functionalStateIsKnown(wire))
    return false;
  return *wire == static_cast<int>(FunctionalState::Faulted);
}

inline QList<QString> stateRejectionTexts(std::optional<int> wire) {
  QList<QString> reasons;
  if (!wire.has_value() || *wire == 0)
    return reasons;
  for (const CarStateLabel &entry : STATE_REJECTION_LABELS) {
    if (*wire & entry.value)
      reasons.append(QString::fromUtf8(entry.label));
  }
  if (*wire & ~STATE_REJECTION_KNOWN_BITS)
    reasons.append(QStringLiteral(CAR_STATE_UNKNOWN_REASON_LABEL));
  return reasons;
}

inline bool stateRejectionIsActive(std::optional<int> wire) {
  return wire.has_value() && *wire != 0;
}

#endif // VCU_STATE_LABELS_H
