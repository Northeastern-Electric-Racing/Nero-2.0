#include "debugtablecontroller.h"

DebugTableController::DebugTableController(Model *model, QObject *parent)
    : ButtonController{model, parent} {
  connect(model, &Model::onCurrentDataChange, this,
          &DebugTableController::update);
}

QList<QJsonObject> DebugTableController::topics() const {
  return this->m_topics;
}

QList<QJsonObject> DebugTableController::selectedValues() const {
  return this->m_selectedValues;
}

void DebugTableController::setSelectedValues(QList<QJsonObject> values) {
  this->m_selectedValues = values;
  emit this->selectedValuesChanged();
}

void DebugTableController::setTopics(QList<QJsonObject> topics) {
  this->m_topics = topics;
  emit this->topicsChanged();
}

void DebugTableController::enterButtonPressed() {
  // TODO SELECT CURRENTLY SELECTED VALUE
}

void DebugTableController::downButtonPressed() {
  if (this->scrollingTopics) {
    if (this->selectedTopicIndex < this->m_topics.count() - 1) {
      this->selectedTopicIndex++;
    } else {
      if (this->selectedValueIndex < this->m_selectedValues.count() - 1) {
        this->selectedValueIndex++;
      }
    }
  }
}

void DebugTableController::upButtonPressed() {
  if (this->scrollingTopics) {
    if (this->selectedTopicIndex != 0) {
      this->selectedTopicIndex--;
    }
  } else {
    if (this->selectedValueIndex != 0) {
      this->selectedTopicIndex--;
    }
  }
}

void DebugTableController::leftButtonPressed() {
  this->scrollingTopics = false;
}

void DebugTableController::rightButtonPressed() {
  this->scrollingTopics = true;
}

void DebugTableController::update() {
  QSet<QString> topicsSet = {};
  QList<QString> topics = {};
  for (const DebugTableRowValue &key : this->m_model->getDebugTableValues()) {
    QVector<QString> split = key.name().split("/");

    if (split.length() > 1) {

      QString topic = split[0] + "/" + split[1];
      if (!topicsSet.contains(topic)) {
        topicsSet.insert(topic);
        topics.append(topic);
      }
    }
  }
  if (topics.length() == 0)
    return;

  QString selectedTopic = topics[this->selectedTopicIndex];

  QList<DebugTableRowValue> selectedValues = {};

  for (const DebugTableRowValue &key : this->m_model->getDebugTableValues()) {
    if (key.name().contains(selectedTopic)) {
      selectedValues.append(key);
    }
  }

  DebugTableRowValues newSelectedValues;
  DebugTableRowTopics newTopics;
  newTopics.setTopics(topics);
  newSelectedValues.setDebugTableRowValues(selectedValues);
  qDebug() << newTopics.json() << newSelectedValues.json();
  this->setTopics(newTopics.json());
  this->setSelectedValues(newSelectedValues.json());
}
