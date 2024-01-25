#ifndef SIGNALSCONTROLLER_H
#define SIGNALSCONTROLLER_H
#include <QObject>
#include <QMap>
#include <QString>

/**
 * @brief The TheSignalsController class
 * Controller for the signals view
 */
class SignalsController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QMap<QString, QString> attributeStatus READ getAttributeStatus NOTIFY attributeStatusChanged)
public:
    explicit SignalsController(QObject *parent = nullptr);

    QMap<QString, QString> getAttributeStatus(const QString &attribute) const;
signals:
    void attributeStatusChanged(const QMap<QString, QString> &attributeStatus);
public slots:
    void setAttributeStatus(const QString &attribute, const QString &status);
private:
    QMap<QString, QString> m_attributeStatus;

};



#endif // SIGNALSCONTROLLER_H
