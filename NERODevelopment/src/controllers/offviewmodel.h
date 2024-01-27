#ifndef OFFVIEWMODEL_H
#define OFFVIEWMODEL_H

#include <QObject>
#include "AttributeNames.h"

class OffViewModel : public QObject {
    Q_OBJECT
public:
    explicit OffViewModel(QObject *parent = nullptr);
    AttributeStatus getAttributeStatus(AttributeName attributeName) const;

private:
    // Define member variables to store model data
    QMap<AttributeName, AttributeStatus> m_attributeStatusMap;
};

#endif // OFFVIEWMODEL_H
