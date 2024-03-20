#include "debugtablecontroller.h"

DebugTableController::DebugTableController(Model *model, QObject *parent)
    : ButtonController{model, 2, parent} {
  connect(model, &Model::onCurrentDataChange, this,
          &DebugTableController::update);
}

QList<QJsonObject> DebugTableController::topics() const {
  return this->m_topics;
}

QList<QJsonObject> DebugTableController::selectedValues() const {
  return this->m_selectedValues;
}

int DebugTableController::selectedValuesIndex() const {
  return this->m_selectedValuesIndex;
}

int DebugTableController::selectedTopicsIndex() const {
  return this->m_selectedTopicsIndex;
}

bool DebugTableController::scrollingTopics() const {
  return this->m_scrollingTopics;
}

void DebugTableController::setScrollingTopics(bool scrollingTopics) {
  if (this->m_scrollingTopics != scrollingTopics) {
    this->m_scrollingTopics = scrollingTopics;
    emit this->scrollingTopicsChanged();
  }
}

void DebugTableController::setSelectedValuesIndex(int index) {
  if (this->m_selectedValuesIndex != index) {
    this->m_selectedValuesIndex = index;
    emit this->selectedValuesIndexChanged();
  }
}

void DebugTableController::setSelectedTopicsIndex(int index) {
  if (this->m_selectedTopicsIndex != index) {
    this->m_selectedTopicsIndex = index;
    emit this->selectedTopicsIndexChanged();
  }
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
  qDebug() << "pressed down arrow" << m_selectedTopicsIndex << m_scrollingTopics
           << this->m_topics.length() << m_selectedValues.length()
           << m_selectedValuesIndex;
  if (this->m_scrollingTopics) {
    if (this->m_selectedTopicsIndex < this->m_topics.length() - 1) {
      this->setSelectedTopicsIndex(this->m_selectedTopicsIndex + 1);
      this->update();
    }
  } else {
    if (this->m_selectedValuesIndex < this->m_selectedValues.length() - 1) {
      this->setSelectedValuesIndex(this->m_selectedValuesIndex + 1);
    }
  }
}

void DebugTableController::upButtonPressed() {
  if (this->m_scrollingTopics) {
    if (this->m_selectedTopicsIndex != 0) {
      this->setSelectedTopicsIndex(this->m_selectedTopicsIndex - 1);
      this->update();
    }
  } else {
    if (this->m_selectedValuesIndex != 0) {
      this->setSelectedValuesIndex(this->m_selectedValuesIndex - 1);
    }
  }
}

void DebugTableController::leftButtonPressed() {
  this->setScrollingTopics(true);
}

void DebugTableController::rightButtonPressed() {
  this->setScrollingTopics(false);
}

void DebugTableController::update() {
  if (this->m_pageIndex != this->m_model->currentPageIndex) {
    return;
  }
  QSet<QString> topicsSet = {};
  QList<QString> topics = {};
  QList<DebugTableRowValue> rows = this->m_model->getDebugTableValues();
  for (const DebugTableRowValue &row : rows) {
    QVector<QString> split = row.name().split("/");

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

  QString selectedTopic = topics[this->m_selectedTopicsIndex];

  QList<DebugTableRowValue> selectedValues = {};

  // for (const DebugTableRowValue &row : rows) {
  //   if (row.name().contains(selectedTopic)) {
  //     selectedValues.append(row);
  //   }
  // }

  DebugTableRowValues newSelectedValues;
  DebugTableRowTopics newTopics;
  newTopics.setTopics(topics);
  newSelectedValues.setDebugTableRowValues(selectedValues);

  // qDebug() << newTopics.json() << newSelectedValues.json();

  this->setTopics(newTopics.json());
  this->setSelectedValues(newSelectedValues.json());
}
